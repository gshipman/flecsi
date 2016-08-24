/*~--------------------------------------------------------------------------~*
 *  @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
 * /@@/////  /@@          @@////@@ @@////// /@@
 * /@@       /@@  @@@@@  @@    // /@@       /@@
 * /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
 * /@@////   /@@/@@@@@@@/@@       ////////@@/@@
 * /@@       /@@/@@//// //@@    @@       /@@/@@
 * /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
 * //       ///  //////   //////  ////////  //
 *
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_serial_tuple_h
#define flecsi_serial_tuple_h

//----------------------------------------------------------------------------//
// POLICY_NAMESPACE must be defined before including storage_type.h!!!
// Using this approach allows us to have only one storage_type_t
// definintion that can be used by all data policies -> code reuse...
#define POLICY_NAMSPACE serial
#include "flecsi/data/storage_type.h"
#undef POLICY_NAMESPACE
//----------------------------------------------------------------------------//

#include "flecsi/utils/const_string.h"

///
// \file serial/tuple.h
// \authors bergen
// \date Initial file creation: Apr 17, 2016
///

namespace flecsi {
namespace data {
namespace serial {

  ///
  // FIXME: Tuple storage type.
  ///
  template<typename data_store_t, typename meta_data_t>
  struct storage_type_t<tuple, data_store_t, meta_data_t> {

    struct tuple_accessor_t {
    }; // struct tuple_accessor_t

    struct tuple_handle_t {
    }; // struct tuple_handle_t

    template<
      typename T,
      size_t NS,
      typename ... Args
    >
    static
    decltype(auto)
    register_data(
      data_store_t & data_store,
      uintptr_t runtime_namespace,
      const const_string_t & key,
      size_t indices,
      Args && ... args
    )
    {
    } // register_data

    ///
    //
    ///
    template<
      typename T,
      size_t NS
    >
    static
    tuple_accessor_t
    get_accessor(
      data_store_t & data_store,
      uintptr_t runtime_namespace,
      const const_string_t & key
    )
    {
      return {};
    } // get_accessor

    ///
    //
    ///
    template<
      typename T,
      size_t NS
    >
    static
    tuple_handle_t
    get_handle(
      data_store_t & data_store,
      uintptr_t runtime_namespace,
      const const_string_t & key
    )
    {
      return {};
    } // get_handle

  }; // struct storage_type_t

} // namespace serial
} // namespace data
} // namespace flecsi

#endif // flecsi_serial_tuple_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
