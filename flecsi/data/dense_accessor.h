/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_dense_accessor_h
#define flecsi_dense_accessor_h

#include "flecsi/data/accessor.h"
#include "flecsi/data/data_constants.h"
#include "flecsi/data/data_handle.h"

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Nov 13, 2017
//----------------------------------------------------------------------------//

namespace flecsi {

//----------------------------------------------------------------------------//
//! The dense_accessor_base_t type provides an empty base type for compile-time
//! identification of data handle objects.
//!
//! @ingroup data
//----------------------------------------------------------------------------//

struct dense_accessor_base_t : public accessor_base_t {};

//----------------------------------------------------------------------------//
//! The dense_accessor__ type captures information about permissions
//! and specifies a data policy.
//!
//! @tparam T                     The data type referenced by the handle.
//! @tparam EXCLUSIVE_PERMISSIONS The permissions required on the exclusive
//!                               indices of the index partition.
//! @tparam SHARED_PERMISSIONS    The permissions required on the shared
//!                               indices of the index partition.
//! @tparam GHOST_PERMISSIONS     The permissions required on the ghost
//!                               indices of the index partition.
//! @tparam DATA_POLICY           The data policy for this handle type.
//!
//! @ingroup data
//----------------------------------------------------------------------------//

template<
  typename T,
  size_t EXCLUSIVE_PERMISSIONS,
  size_t SHARED_PERMISSIONS,
  size_t GHOST_PERMISSIONS
>
struct dense_accessor__ : public dense_accessor_base_t {
  using handle_t = 
    data_handle__<
      T,
      EXCLUSIVE_PERMISSIONS,
      SHARED_PERMISSIONS,
      GHOST_PERMISSIONS
    >;

  //--------------------------------------------------------------------------//
  //! Copy constructor.
  //--------------------------------------------------------------------------//

  dense_accessor__(const data_handle__<T, 0, 0, 0>& h)
  : handle(reinterpret_cast<const handle_t&>(h)){

  }

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  T& operator()(size_t index){
    assert(index < handle.combined_size && "index out of range");
    #ifndef MAPPER_COMPACTION
    #ifndef COMPACTED_STORAGE_SORT
        return handle.combined_data[index];
    #else
        return handle.combined_data_sort[index];
    #endif
    #else
        return *(handle.combined_data+index);
    #endif
  }

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  const T& operator()(size_t index) const{
    return const_cast<dense_accessor__&>(*this)(index);
  }

  ///
  // \brief Return the index space size of the data variable
  //        referenced by this handle.
  ///
  size_t size() const{
    return handle.combined_size;
  }

  ///
  // \brief Return the index space size of the data variable
  //        referenced by this handle.
  ///
  size_t
  exclusive_size() const
  {
    return handle.exclusive_size;
  } // size

  ///
  // \brief Return the index space size of the data variable
  //        referenced by this handle.
  ///
  size_t
  shared_size() const
  {
    return handle.shared_size;
  } // size

  ///
  // \brief Return the index space size of the data variable
  //        referenced by this handle.
  ///
  size_t
  ghost_size() const
  {
    return handle.ghost_size;
  } // size

  //--------------------------------------------------------------------------//
  // Operators.
  //--------------------------------------------------------------------------//

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  const T &
  exclusive (
    size_t index
  ) const
  {
    assert(index < handle.exclusive_size && "index out of range");
    return handle.exclusive_data[index];
  } // operator []

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  T &
  exclusive (
    size_t index
  )
  {
    assert(index < handle.exclusive_size && "index out of range");
    return handle.exclusive_data[index];
  } // operator []

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  const T &
  shared (
    size_t index
  ) const
  {
    assert(index < handle.shared_size && "index out of range");
    return handle.shared_data[index];
  } // operator []

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  T &
  shared (
    size_t index
  )
  {
    assert(index < handle.shared_size && "index out of range");
    return handle.shared_data[index];
  } // operator []

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  const T &
  ghost (
    size_t index
  ) const
  {
    assert(index < handle.ghost_size && "index out of range");
    return handle.ghost_data[index];
  } // operator []

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \param index The index of the data variable to return.
  ///
  T &
  ghost (
    size_t index
  )
  {
    assert(index < handle.ghost_size && "index out of range");
    return handle.ghost_data[index];
  } // operator []

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \tparam E A complex index type.
  //
  // This version of the operator is provided to support use with
  // \e flecsi mesh entity types \ref mesh_entity_base_t.
  ///
  template<typename E>
  const T &
  operator () (
    E * e
  ) const
  {
    return this->operator()(e->template id<0>());
  } // operator ()

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \tparam E A complex index type.
  //
  // This version of the operator is provided to support use with
  // \e flecsi mesh entity types \ref mesh_entity_base_t.
  ///
  template<typename E>
  T &
  operator () (
    E * e
  )
  {
    return this->operator()(e->template id<0>());
  } // operator ()

  handle_t handle;  
};

template<
  typename T,
  size_t PERMISSIONS
>
struct global_accessor__ : public dense_accessor__<T, PERMISSIONS, 0, 0>{
  using base_t = dense_accessor__<T, PERMISSIONS, 0, 0>;

  global_accessor__(const data_handle__<T, 0, 0, 0>& h)
  : base_t(h){}

  operator T&(){
    return data();
  }

  operator const T&() const{
    return data();
  }

  T& data() const{
    return *base_t::handle.combined_data;
  }

  global_accessor__&
  operator=(const T& x){
    data() = x;
    return *this;
  }

  //--------------------------------------------------------------------------/
  //
  // Operators.
  //--------------------------------------------------------------------------/
  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \tparam E A complex index type.
  //
  // This version of the operator is provided to support use with
  // \e flecsi mesh entity types \ref mesh_entity_base_t.
  ///
  template<typename E>
  const T &
  operator () (
    E * e
  ) const
  {
    return this->operator()(e->template id<0>());
  } // operator ()

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \tparam E A complex index type.
  //
  // This version of the operator is provided to support use with
  // \e flecsi mesh entity types \ref mesh_entity_base_t.
  ///
  template<typename E>
  T &
  operator () (
    E * e
  )
  {
    return this->operator()(e->template id<0>());
  } // operator ()
};

template<
  typename T,
  size_t PERMISSIONS
>
struct color_accessor__ : public dense_accessor__<T, PERMISSIONS, 0, 0>{
  using base_t = dense_accessor__<T, PERMISSIONS, 0, 0>;

  color_accessor__(const data_handle__<T, 0, 0, 0>& h)
  : base_t(h){}

  operator T&(){
    return data();
  }

  operator const T&() const{
    return data();
  }

  T& data() const{
    return *base_t::handle.combined_data;
  }

  color_accessor__&
  operator=(const T& x){
    data() = x;
    return *this;
  }

  //--------------------------------------------------------------------------/
  //
  // Operators.
  //--------------------------------------------------------------------------/
  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \tparam E A complex index type.
  //
  // This version of the operator is provided to support use with
  // \e flecsi mesh entity types \ref mesh_entity_base_t.
  ///
  template<typename E>
  const T &
  operator () (
    E * e
  ) const
  {
    return this->operator()(e->template id<0>());
  } // operator ()

  ///
  // \brief Provide logical array-based access to the data for this
  //        data variable.  This is the const operator version.
  //
  // \tparam E A complex index type.
  //
  // This version of the operator is provided to support use with
  // \e flecsi mesh entity types \ref mesh_entity_base_t.
  ///
  template<typename E>
  T &
  operator () (
    E * e
  )
  {
    return this->operator()(e->template id<0>());
  } // operator ()
};

template<
  typename T,
  size_t EXCLUSIVE_PERMISSIONS,
  size_t SHARED_PERMISSIONS,
  size_t GHOST_PERMISSIONS
>
struct accessor_type__<
  data::storage_label_type_t::dense,
  T,
  EXCLUSIVE_PERMISSIONS,
  SHARED_PERMISSIONS,
  GHOST_PERMISSIONS
>
{
  using type = 
    dense_accessor__<T, EXCLUSIVE_PERMISSIONS, SHARED_PERMISSIONS,
    GHOST_PERMISSIONS>;
};

template<
  typename T,
  size_t EXCLUSIVE_PERMISSIONS,
  size_t SHARED_PERMISSIONS,
  size_t GHOST_PERMISSIONS
>
struct accessor_type__<
  data::storage_label_type_t::color,
  T,
  EXCLUSIVE_PERMISSIONS,
  SHARED_PERMISSIONS,
  GHOST_PERMISSIONS
>
{
  using type = 
    color_accessor__<T, EXCLUSIVE_PERMISSIONS>;
};

template<
  typename T,
  size_t EXCLUSIVE_PERMISSIONS,
  size_t SHARED_PERMISSIONS,
  size_t GHOST_PERMISSIONS
>
struct accessor_type__<
  data::storage_label_type_t::global,
  T,
  EXCLUSIVE_PERMISSIONS,
  SHARED_PERMISSIONS,
  GHOST_PERMISSIONS
>
{
  using type = 
    global_accessor__<T, EXCLUSIVE_PERMISSIONS>;
};

template<
  typename T,
  size_t EXCLUSIVE_PERMISSIONS,
  size_t SHARED_PERMISSIONS,
  size_t GHOST_PERMISSIONS
>
using dense_accessor = 
  typename accessor_type__<
  data::storage_label_type_t::dense, T, EXCLUSIVE_PERMISSIONS,
    SHARED_PERMISSIONS, GHOST_PERMISSIONS>::type;

template<
  typename T,
  size_t PERMISSIONS
>
using color_accessor = 
  typename accessor_type__<
  data::storage_label_type_t::color, T, PERMISSIONS, 0, 0>::type;

template<
  typename T,
  size_t PERMISSIONS
>
using global_accessor = 
  typename accessor_type__<
  data::storage_label_type_t::global, T, PERMISSIONS, 0, 0>::type;

} // namespace flecsi

#endif // flecsi_dense_accessor_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
