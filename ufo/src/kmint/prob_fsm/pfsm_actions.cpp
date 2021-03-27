#include "kmint/prob_fsm/pfsm_actions.hpp"

#include <iostream>

#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/random.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo::student {

void pfsm_actions::ExecuteAction(tank& t) {
    switch (t.state) {
        case tState::wander:
            pfsm_actions::Execute_Wander(t);
            break;
        case tState::flee:
            pfsm_actions::Execute_Flee(t);
            break;
        case tState::gotoEMP:
            pfsm_actions::Execute_GoToEMP(t);
            break;
        case tState::gotoShield:
            pfsm_actions::Execute_GoToShield(t);
            break;
        case tState::repair:
            pfsm_actions::Execute_Repair(t);
            break;
        case tState::damageLookout:
            return;
    }
}

void pfsm_actions::Execute_Wander(tank& t) {
    if (t.weight < 1) {
        t.next_edge = &t.node()[random_int(0, t.node().num_edges())];
    }
}

void pfsm_actions::Execute_Repair(tank& t) { t.GoToRepair(); }

void pfsm_actions::Execute_Flee(tank& t) {
    play::actor* ToFleeFrom = t.GetNearestUFO(t.ufos);

    t.MoveAwayFrom(ToFleeFrom);
    t.fleecount++;
}

void pfsm_actions::Execute_GoToEMP(tank& t) { t.GoTo(pickup_type::EMP); }

void pfsm_actions::Execute_GoToShield(tank& t) { t.GoTo(pickup_type::SHIELD); }

#pragma region helpers

#pragma endregion helpers

}  // namespace kmint::ufo::student