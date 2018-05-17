#pragma once

#include <flecsi-config.h>

#if !defined(FLECSI_ENABLE_LEGION)
#error FLECSI_ENABLE_LEGION not defined! This file depends on Legion!
#endif

#include <legion.h>
#include <flecsi/execution/context.h>

namespace flecsi {
namespace execution {

class legion_entity {
public:
  Legion::LogicalRegion logical_region;
  Legion::IndexSpace index_space;
  Legion::FieldSpace field_space;
  Legion::LogicalPartition equal_lp;
  Legion::FutureMap task_fm;
  Legion::FieldID color_fid;
  Legion::FieldID id_fid;
  Legion::FieldID offset_fid;
  int id;
public:
  legion_entity() 
  {
  }
};

class legion_adjacency {
public:
  Legion::LogicalRegion logical_region;
  Legion::IndexSpace index_space;
  Legion::FieldSpace field_space;
  Legion::FieldID image_nrange_fid;
  Legion::FieldID image_fid;
public:
  legion_adjacency() 
  {
  }
};

class legion_partition {
public:
  Legion::IndexPartition index_partition;
  Legion::LogicalPartition logical_partition;
public:
  legion_partition() {}
};

class dependent_partition {
public:
  int num_color;
  Legion::IndexSpace partition_index_space;
  Legion::Domain color_domain;
public:
  dependent_partition() {}
  
  legion_entity load_cell(int num_cells);
  legion_entity load_non_cell(int num_entities, int entity_id);
  legion_adjacency load_cell_to_cell(legion_entity &cell_region);
  legion_adjacency load_cell_to_others(legion_entity &cell_region, legion_entity &other_region);
  legion_partition partition_by_color(legion_entity &entity);
  legion_partition partition_by_image(legion_entity &from_entity, legion_entity &to_entity, legion_adjacency &adjacency, legion_partition &from);
  legion_partition partition_by_difference(legion_entity &entity, legion_partition &par1, legion_partition &par2);
  legion_partition partition_by_intersection(legion_entity &entity, legion_partition &par1, legion_partition &par2);
  void output_partition(legion_entity &entity, legion_partition &primary, legion_partition &ghost, legion_partition &shared, legion_partition &exclusive);
  void set_offset(legion_entity &entity, legion_partition &primary);
  void min_reduction_color(legion_entity &entity, legion_partition &alias_partition);
};
  
} // namespace execution
} // namespace flecsi