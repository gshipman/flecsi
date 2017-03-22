/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2017 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

///
// \file legion/handle_policy.h
// \authors nickm
// \date Initial file creation: Feb 27, 2017
///

#ifndef flecsi_legion_handle_policy_h
#define flecsi_legion_handle_policy_h

#include "legion.h"

namespace flecsi {
namespace data {

struct legion_handle_policy_t
{
  size_t exclusive_priv;
  size_t shared_priv;
  size_t ghost_priv;
  Legion::LogicalRegion lr;
  Legion::IndexPartition exclusive_ip;
  Legion::IndexPartition shared_ip;
  Legion::IndexPartition ghost_ip;
  Legion::LogicalRegion exclusive_lr;
  Legion::LogicalRegion shared_lr;
  Legion::LogicalRegion ghost_lr;
  Legion::PhysicalRegion exclusive_pr;
  void* exclusive_data;
  Legion::PhysicalRegion shared_pr;
  void* shared_data;
  Legion::PhysicalRegion ghost_pr;
  void* ghost_data;
  Legion::Context context;
  Legion::Runtime* runtime;
  
  //Legion::PhaseBarrier pbarrier_as_master;
  //std::vector<Legion::PhaseBarrier> masters_pbarriers;

  Legion::PhaseBarrier* pbarrier_as_master_ptr;
  std::vector<Legion::PhaseBarrier*> masters_pbarriers_ptrs;

  std::vector<Legion::LogicalRegion> lregions_neighbors_shared;
  std::vector<Legion::PhysicalRegion> pregions_neighbors_shared;
  Legion::LogicalRegion lregion_ghost;
  size_t ghost_copy_task_id;
  Legion::FieldID ghost_fid;
  bool is_readable;
};

} // namespace data
} // namespace flecsi

#endif // flecsi_legion_handle_policy_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
