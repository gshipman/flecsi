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

/*!  @file */

#if !defined(__FLECSI_PRIVATE__)
#error Do not include this file directly!
#endif

#if !defined(FLECSI_ENABLE_LEGION)
#error FLECSI_ENABLE_LEGION not defined! This file depends on Legion!
#endif

#include "../field_info.hh"
#include <legion.h>

#include <unordered_map>

namespace flecsi {
namespace data {

/*----------------------------------------------------------------------------*
  Global Topology.
 *----------------------------------------------------------------------------*/

namespace global_topo {

struct runtime_data_t {
  size_t index_space_id;
  Legion::IndexSpace index_space;
  Legion::FieldSpace field_space;
  Legion::LogicalRegion logical_region;
}; // struct runtime_data_t

} // namespace global_topo

/*----------------------------------------------------------------------------*
  Index Topology.
 *----------------------------------------------------------------------------*/

namespace index_topo {

struct runtime_data_t {
  size_t colors;
  size_t index_space_id;
  Legion::IndexSpace index_space;
  Legion::FieldSpace field_space;
  Legion::LogicalRegion logical_region;
  Legion::LogicalPartition color_partition;
}; // struct runtime_data_t

} // namespace index_topo

/*----------------------------------------------------------------------------*
  Unstructured Mesh Topology.
 *----------------------------------------------------------------------------*/

#if 0
struct unstructured_mesh_runtime_data_t {
  std::vector<base_data_t> entities;
  std::vector<base_data_t> adjacencies;
  std::vector<Legion::LogicalPartition> exclusive;
  std::vector<Legion::LogicalPartition> shared;
  std::vector<Legion::LogicalPartition> ghost;
  std::vector<Legion::LogicalPartition> ghost_owners;
}; // struct unstructured_mesh_runtime_data_t
#endif

namespace unstructured_mesh {

struct runtime_data_t {}; // struct runtime_data_t

} // namespace unstructured_mesh

#if 0
struct unstructured_mesh_dense_runtime_data_t {
}; // struct unstructured_mesh_dense_runtime_data_t

struct unstructured_mesh_ragged_runtime_data_t {
}; // struct unstructured_mesh_ragged_runtime_data_t

struct unstructured_mesh_sparse_runtime_data_t {
}; // struct unstructured_mesh_sparse_runtime_data_t

struct unstructured_mesh_subspace_runtime_data_t {
}; // struct unstructured_mesh_subspace_runtime_data_t

struct structured_mesh_runtime_data_t {
}; // struct structured_mesh_runtime_data_t
#endif

} // namespace data
} // namespace flecsi