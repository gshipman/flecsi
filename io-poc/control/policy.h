#pragma once

#include <tuple>

#include <flecsi/control/control.h>
#include <flecsi/utils/typeify.h>

#include <io-poc/control/node_type.h>

namespace io_poc {

/*----------------------------------------------------------------------------*
 * Define simulation phases.
 *----------------------------------------------------------------------------*/

enum simulation_phases_t : size_t {
  initialize,
  advance,
  analyze,
  io,
  mesh,
  finalize
}; // enum simulation_phases_t

/*----------------------------------------------------------------------------*
 * Define action attributes.
 *----------------------------------------------------------------------------*/

enum action_attributes_t : size_t {
  time_advance_half = 0x01,
  time_advance_whole = 0x02,
  updated_eos_at_faces = 0x04
}; // enum action_attributes_t

/*----------------------------------------------------------------------------*
 * Control policy.
 *----------------------------------------------------------------------------*/

struct control_policy_t {

  using control_t = flecsi::control::control_u<control_policy_t>;

  using node_t = io_poc::node_t;

  static bool evolve_control() {
    return control_t::instance().step()++ < 5;
  } // evolve

#define phase(name) flecsi::control::phase_<name>

  using evolve = flecsi::control::cycle_u<evolve_control, // stopping predicate
    phase(advance),
    phase(analyze),
    phase(io),
    phase(mesh)>;

  using phases = std::tuple<phase(initialize), evolve, phase(finalize)>;

  size_t & step() {
    return step_;
  }

private:
  size_t step_;

}; // struct control_policy_t

} // namespace io_poc
