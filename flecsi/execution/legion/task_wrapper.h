/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_execution_legion_task_wrapper_h
#define flecsi_execution_legion_task_wrapper_h

#include "flecsi/data/data_handle.h"
#include "flecsi/data/accessor.h"
#include "flecsi/execution/context.h"
#include "flecsi/execution/legion/task_args.h"

#if FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_mpilegion
  #include "flecsi/execution/mpilegion/legion_handshake.h"
  #include "flecsi/data/legion/dense.h"
#else
  #include "flecsi/data/serial/dense.h"
#endif

#include "flecsi/utils/common.h"
#include "flecsi/utils/tuple_type_converter.h"

///
// \file legion/task_wrapper.h
// \authors bergen
// \date Initial file creation: Jul 24, 2016
///

template<size_t I, typename AS, typename PS>
struct create_task_args__{
  static size_t
  create(LegionRuntime::HighLevel::Context context,
         LegionRuntime::HighLevel::HighLevelRuntime * runtime,
         const std::vector<
           LegionRuntime::HighLevel::PhysicalRegion>& regions, AS& args){
    
    using type = 
      typename std::tuple_element<std::tuple_size<AS>::value - I,
        PS>::type;

    handle_<type>(context, runtime, regions,
                  std::get<std::tuple_size<AS>::value - I>(args));

    return create_task_args__<I - 1, AS, PS>::
      create(context, runtime, regions, args);
  }

  template<typename PT>
  static void
  handle_(LegionRuntime::HighLevel::Context context,
          LegionRuntime::HighLevel::HighLevelRuntime* runtime,
          const std::vector<
           LegionRuntime::HighLevel::PhysicalRegion>& regions,
                      flecsi::data_handle_t<void, 0>& h){
    flecsi::execution::field_ids_t & fid_t = 
      flecsi::execution::field_ids_t::instance();

    using type = typename PT::type;

    auto pr = regions[h.region];

    auto ac = pr.get_field_accessor(fid_t.fid_value).typeify<type>();
    
    Legion::LogicalRegion lr = pr.get_logical_region();
    Legion::IndexSpace is = lr.get_index_space();
    Legion::Domain domain = runtime->get_index_space_domain(context, is);
    LegionRuntime::Arrays::Rect<1> r = domain.get_rect<1>();
    LegionRuntime::Arrays::Rect<1> sr;
    LegionRuntime::Accessor::ByteOffset bo[1];
    h.data = ac.template raw_rect_ptr<1>(r, sr, bo);
  }

  template<typename PT, typename R>
  static
  typename std::enable_if_t<!std::is_base_of<flecsi::data_handle_base, R>::
    value>
  handle_(LegionRuntime::HighLevel::Context,
         LegionRuntime::HighLevel::HighLevelRuntime*,
          const std::vector<
           LegionRuntime::HighLevel::PhysicalRegion>&, R&){}
};

template<typename AS, typename PS>
struct create_task_args__<0, AS, PS>{
  static size_t
  create(LegionRuntime::HighLevel::Context context,
        LegionRuntime::HighLevel::HighLevelRuntime* runtime,
         const std::vector<
           LegionRuntime::HighLevel::PhysicalRegion>&, AS& args){
    return 0;
  }
};

namespace flecsi {
namespace execution {

///
// \brief
//
// \tparam P processor type
// \tparam S single type task
// \tparam I index type task
// \tparam R task return type
// \tparam As arguments
///
template<
  processor_t P,
  bool S,
  bool I,
  typename R,
  typename A
>
struct legion_task_wrapper__
{
  using user_task_args_t = 
    typename utils::base_convert_tuple_type<accessor_base, 
    data_handle_t<void, 0>, A>::type;

  //
  // Type definition for user task.
  //
  using task_args_t = legion_task_args__<R,A,user_task_args_t>;
  using user_task_handle_t = typename task_args_t::user_task_handle_t;

  using lr_runtime = LegionRuntime::HighLevel::HighLevelRuntime;
  using lr_proc = LegionRuntime::HighLevel::Processor;
  using task_id_t = LegionRuntime::HighLevel::TaskID;

  using user_args_t = 
    typename utils::base_convert_tuple_type<accessor_base, 
    data_handle_t<void, 0>, A>::type;

#ifdef DH1
    //typename user_args_t::a x;
#endif

  //
  // This defines a predicate function to pass to tuple_filter that
  // will select all tuple elements after the first index, i.e., 0.
  //
  template<typename T>
  using greater_than = std::conditional_t<(T()>0), std::true_type,
    std::false_type>;

  template<typename T>
  using is_data_handle = std::is_base_of<data_handle_base,T>;

  ///
  // This function is called by the context singleton to do the actual
  // registration of the task wrapper with the Legion runtime. The structure
  // of the logic used is really just an object factory pattern.
  ///
  static
  void
  register_task(
    task_id_t tid
  )
  {
    switch(P) {
      case loc:
        lr_runtime::register_legion_task<R, execute>(
          tid, lr_proc::LOC_PROC, S, I);
        break;
      case toc:
        lr_runtime::register_legion_task<R, execute>(
          tid, lr_proc::TOC_PROC, S, I);
        break;
      case mpi:
        break;
    } // switch
  } // register_task

  ///
  // This method executes the user's task after processing the arguments
  // from the Legion runtime.
  ///
  static R execute(const LegionRuntime::HighLevel::Task * task,
    const std::vector<LegionRuntime::HighLevel::PhysicalRegion> & regions,
    LegionRuntime::HighLevel::Context context,
    LegionRuntime::HighLevel::HighLevelRuntime * runtime)
  {

    // Unpack task arguments
    task_args_t & task_args = *(reinterpret_cast<task_args_t *>(task->args));
    user_task_handle_t & user_task_handle = task_args.user_task_handle;
    user_task_args_t & user_task_args = task_args.user_args;

    // ndm - the accessor ctor will be called with data handle

    // Push the Legion state
    context_t::instance().push_state(user_task_handle.key,
      context, runtime, task, regions);

    #ifdef DH1
        //typename user_args_t::a x;
    #endif

#if 0
    // FIXME: Working on processing data handles
    // Somehow (???) we are going to have to interleave the processed
    // data handle arguments back into the original slots...

    // Get the data handle task arguments
    auto data_args = tuple_filter_<is_data_handle, task_args_t>(task_args);
    std::cout << "data_args size: " <<
      std::tuple_size<decltype(data_args)>::value << std::endl;

    utils::tuple_for_each(data_args, [&](auto & element) {
      std::cout << "hello" << std::endl;
    });

    // Execute the user task
    return tuple_function(user_task, user_args);
#endif

    auto retval = user_task_handle(
      context_t::instance().function(user_task_handle.key),
      user_task_args);

    // Pop the Legion state
    context_t::instance().pop_state(user_task_handle.key);
    
    return retval;
  } // execute

#if 0
  static R execute_mpi(const LegionRuntime::HighLevel::Task * task,
    const std::vector<LegionRuntime::HighLevel::PhysicalRegion>& regions,
    LegionRuntime::HighLevel::Context context,
    LegionRuntime::HighLevel::HighLevelRuntime * runtime)
  {
#ifdef LEGIONDEBUG
     int rank;
     MPI_Comm_rank( MPI_COMM_WORLD, &rank);
     std::cout<<"MPI rank from the index task = " << rank <<std::endl;
     ext_legion_handshake_t::instance().rank_=rank;
#endif

    task_args_t & task_args = *(reinterpret_cast<task_args_t *>(task->args));
    user_task_handle_t & user_task_handle = task_args.user_task_handle;
    user_task_args_t & user_task_args = task_args.user_args;

    // Get the user task arguments
//  auto user_args = tuple_filter_index_<greater_than, task_args_t>(task_args);
 
    auto bound_user_task = std::bind(reinterpret_cast<std::function<R(A)> *>(
      context_t::instance().function(user_task_handle.key)), user_task_args);

     ext_legion_handshake_t::instance().shared_func_ = bound_user_task;

     ext_legion_handshake_t::instance().call_mpi_=true;
  } // execute_mpi
#endif

}; // class legion_task_wrapper__

///
// Partial specialization for void.
///
template<
  processor_t P,
  bool S,
  bool I,
  typename A
>
struct legion_task_wrapper__<P, S, I, void, A>
{
  //
  // Type definition for user task.
  //
  using user_task_args_t = 
    typename utils::base_convert_tuple_type<accessor_base, 
    data_handle_t<void, 0>, A>::type;
  using args_t = A;

  using task_args_t = legion_task_args__<void,A,user_task_args_t>;
  using user_task_handle_t = typename task_args_t::user_task_handle_t;

  using lr_runtime = LegionRuntime::HighLevel::HighLevelRuntime;
  using lr_proc = LegionRuntime::HighLevel::Processor;
  using task_id_t = LegionRuntime::HighLevel::TaskID;

  //
  // This defines a predicate function to pass to tuple_filter that
  // will select all tuple elements after the first index, i.e., 0.
  //
  template<typename T>
  using greater_than = std::conditional_t<(T()>0), std::true_type,
    std::false_type>;

  template<typename T>
  using is_data_handle = std::is_base_of<data_handle_base, T>;

  ///
  // This function is called by the context singleton to do the actual
  // registration of the task wrapper with the Legion runtime. The structure
  // of the logic used is really just an object factory pattern.
  ///
  static
  void
  register_task(
    task_id_t tid
  )
  {
    switch(P) {
      case loc:
        lr_runtime::register_legion_task<execute>(
          tid, lr_proc::LOC_PROC, S, I);
        break;
      case toc:
        lr_runtime::register_legion_task<execute>(
          tid, lr_proc::TOC_PROC, S, I);
        break;
#if FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_mpilegion
      case mpi:
        lr_runtime::register_legion_task<execute_mpi>(
          tid, lr_proc::LOC_PROC, S, I);
        break;
#endif // FLECSI_RUNTIME_MODEL_mpilegion
    } // switch
  } // register_task

  ///
  // This method executes the user's task after processing the arguments
  // from the Legion runtime.
  ///
  static void execute(const LegionRuntime::HighLevel::Task * task,
    const std::vector<LegionRuntime::HighLevel::PhysicalRegion> & regions,
    LegionRuntime::HighLevel::Context context,
    LegionRuntime::HighLevel::HighLevelRuntime * runtime)
  {
    // Unpack task arguments
    task_args_t & task_args = 
      *(reinterpret_cast<task_args_t *>(task->args));
    
    user_task_handle_t & user_task_handle = task_args.user_task_handle;
    user_task_args_t & user_task_args = task_args.user_args;

    // Push the Legion state
    context_t::instance().push_state(user_task_handle.key,
      context, runtime, task, regions);

    create_task_args__<std::tuple_size<user_task_args_t>::value,
      user_task_args_t, args_t>::create(
        context, runtime, regions, user_task_args);

    user_task_handle(context_t::instance().function(user_task_handle.key),
      user_task_args);

    // Pop the Legion state
    context_t::instance().pop_state(user_task_handle.key);
  } // execute

#if FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_mpilegion
  static void execute_mpi(const LegionRuntime::HighLevel::Task * task,
    const std::vector<LegionRuntime::HighLevel::PhysicalRegion>& regions,
    LegionRuntime::HighLevel::Context context,
    LegionRuntime::HighLevel::HighLevelRuntime * runtime)
  {
#ifdef LEGIONDEBUG
     int rank;
     MPI_Comm_rank( MPI_COMM_WORLD, &rank);
     std::cout<<"MPI rank from the index task = " << rank <<std::endl;
     ext_legion_handshake_t::instance().rank_=rank;
#endif

    task_args_t & task_args = *(reinterpret_cast<task_args_t *>(task->args));
    user_task_handle_t & user_task_handle = task_args.user_task_handle;
    user_task_args_t & user_task_args = task_args.user_args;

    // Get the user task arguments
//  auto user_args = tuple_filter_index_<greater_than, task_args_t>(task_args);
 
    std::function<void()> bound_user_task =
      std::bind(*reinterpret_cast<std::function<void(user_task_args_t)> *>(
      context_t::instance().function(user_task_handle.key)), user_task_args);

     ext_legion_handshake_t::instance().shared_func_ = bound_user_task;

     ext_legion_handshake_t::instance().call_mpi_=true;
  } // execute_mpi
#endif // FLECSI_RUNTIME_MODEL_mpilegion

}; // class legion_task_wrapper__

} //namespace execution 
} // namespace flecsi

#endif // flecsi_execution_legion_task_wrapper_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
