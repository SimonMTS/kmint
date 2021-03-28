#include "kmint/finite_state_machine/fsm_transitions.hpp"

#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo::student {

void fsm_transitions::TransitionCheck_TankLookout(saucer& s) {
    if (s.state != State::nomove && fsm_transitions::TankNearby(s)) {
        s.state = State::hunttank;
    }
}

void fsm_transitions::TransitionCheck_Wander(saucer& s) {
    if (fsm_transitions::HumanNearby(s)) {
        s.state = State::hunthuman;
    }
}

void fsm_transitions::TransitionCheck_HuntHuman(saucer& s) {
    if (!fsm_transitions::HumanNearby(s) || s.target == nullptr) {
        s.state = State::wander;
    }
}

void fsm_transitions::TransitionCheck_HuntTank(saucer& s) {
    if (!fsm_transitions::TankNearby(s)) {
        s.state = State::wander;
    }
}

void fsm_transitions::TransitionCheck_NoMove(saucer& s) {
    if (s.seconds_since_move() >= 20) {
        s.state = State::wander;
    }
}

#pragma region helpers

bool fsm_transitions::TankNearby(saucer& s) {
    for (auto it = s.begin_perceived(); it != s.end_perceived(); ++it) {
        if (it->EntityType == "tank") {
            play::actor& tank = *it;
            ufo::tank* tank1 = dynamic_cast<ufo::tank*>(&tank);

            if (tank1->attackable) {
                return true;
            }
        }
    }

    return false;
}

bool fsm_transitions::HumanNearby(saucer& s) {
    for (auto it = s.begin_perceived(); it != s.end_perceived(); ++it) {
        if (it->EntityType == "human") {
            play::actor& human = *it;

            ufo::human* human1 = dynamic_cast<ufo::human*>(&human);
            if (human1->isSafeTank || human1->isSafeHouse) continue;

            return true;
        }
    }

    return false;
}

#pragma endregion helpers

}  // namespace kmint::ufo::student