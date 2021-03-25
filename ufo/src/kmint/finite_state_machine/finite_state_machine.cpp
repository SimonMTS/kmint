#include "kmint/finite_state_machine/finite_state_machine.hpp"

#include <climits>
#include <iostream>

#include "kmint/finite_state_machine/fsm_actions.hpp"
#include "kmint/finite_state_machine/fsm_transitions.hpp"
#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/force_driven_entities/movement_helpers.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo::student {

void finite_state_machine::StateTransitionCheck(saucer& s) {
    fsm_transitions::GlobalStateTransitionCheck(s);

    switch (s.state) {
        case wander:
            fsm_transitions::TransitionCheck_Wander(s);
            break;
        case hunthuman:
            fsm_transitions::TransitionCheck_HuntHuman(s);
            break;
        case hunttank:
            fsm_transitions::TransitionCheck_HuntTank(s);
            break;
        case nomove:
            fsm_transitions::TransitionCheck_NoMove(s);
            break;
    }

    switch (s.state) {
        case wander:
            s.drawable_.set_tint({255, 0, 0});
            break;
        case hunthuman:
            s.drawable_.set_tint({0, 255, 0});
            break;
        case hunttank:
            s.drawable_.set_tint({0, 0, 255});
            break;
        case nomove:
            s.drawable_.set_tint({200, 200, 200});
            break;
    }
}

void finite_state_machine::ExecuteStateAction(saucer& s) {
    math::vector2d force = {0, 0};

    switch (s.state) {
        case wander:
            force = fsm_actions::Execute_Wander(s);
            break;
        case hunthuman:
            force = fsm_actions::Execute_HuntHuman(s);
            break;
        case hunttank:
            force = fsm_actions::Execute_HuntTank(s);
            break;
        case nomove:
            force = fsm_actions::Execute_NoMove(s);
            break;
    }

    fsm_actions::BeamHumans(s);

    movement_helpers::forceFunc stateForceFunc = {
        [force](force_driven_entity& e) { return force; }, 1};

    std::vector<movement_helpers::forceFunc> inputForces{
        {student::movement_helpers::AvoidScreenEdge, INT_MAX}, stateForceFunc};
    student::movement_helpers::MoveTick(s, inputForces);
}

}  // namespace kmint::ufo::student