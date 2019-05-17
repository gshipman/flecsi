/*
    @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
   /@@/////  /@@          @@////@@ @@////// /@@
   /@@       /@@  @@@@@  @@    // /@@       /@@
   /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
   /@@////   /@@/@@@@@@@/@@       ////////@@/@@
   /@@       /@@/@@//// //@@    @@       /@@/@@
   /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
   //       ///  //////   //////  ////////  //

   Copyright (c) 2016, Triad National Security, LLC
   All rights reserved.
                                                                              */
#pragma once

/*!
  @file

  This file is really just a conduit to capture the different
  specializations for data clients and storage classes.
 */

#include <flecsi/data/legion/storage_classes.h>

namespace flecsi {
namespace data {

struct legion_data_policy_t {

  /*--------------------------------------------------------------------------*
    Topology Interface.
   *--------------------------------------------------------------------------*/

  /*
    Capture the base topology type. This is necessary as a place holder in the
    field interface.
   */

  template<typename TOPOLOGY_TYPE>
  using topology_u =
    legion::topology_u<typename TOPOLOGY_TYPE::type_identifier_t>;

#if 0
  template<typename TOPOLOGY_TYPE, size_t NAMESPACE, size_t NAME>
  static decltype(auto) get_topology() {
    using topology_t =
      legion::topology_u<typename TOPOLOGY_TYPE::type_identifier_t>;

    return topology_t::template get_handle<NAMESPACE, NAME>();
  } // get_client_handle
#endif

  /*
    Capture the handle type for the given topology type.
   */

  template<typename TOPOLOGY_TYPE>
  using topology_handle_u = typename legion::topology_u<
    typename TOPOLOGY_TYPE::type_identifier_t>::topology_handle_t;

  /*--------------------------------------------------------------------------*
    Storage Class Interface.
   *--------------------------------------------------------------------------*/

  /*
    Capture the base storage class type. This is necessary as a place holder in
    the field interface.
   */

  template<size_t STORAGE_CLASS, typename TOPOLOGY_TYPE>
  using storage_class_u = legion::storage_class_u<STORAGE_CLASS, TOPOLOGY_TYPE>;

  /*--------------------------------------------------------------------------*
    Topology Accessor Interface.
   *--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*
    Field Accessor Interface.
   *--------------------------------------------------------------------------*/

  template<typename DATA_TYPE, size_t PRIVILEGES>
  using global_accessor_u =
    legion::global_topology::accessor_u<DATA_TYPE, PRIVILEGES>;

  template<typename DATA_TYPE, size_t PRIVILEGES>
  using index_accessor_u =
    legion::index_topology::accessor_u<DATA_TYPE, PRIVILEGES>;

}; // struct legion_data_policy_t

} // namespace data
} // namespace flecsi
