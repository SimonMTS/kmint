#include "kmint/prob_fsm/pfsm_transitions.hpp"

#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo::student {

void pfsm_transitions::TransitionCheck(tState state, tank& tank) {
    switch (state) {
        case tState::wander:
            pfsm_transitions::TransitionCheck_Wander(tank);
            break;
        case tState::flee:
            pfsm_transitions::TransitionCheck_Flee(tank);
            break;
        case tState::gotoEMP:
            pfsm_transitions::TransitionCheck_GoToEMP(tank);
            break;
        case tState::gotoShield:
            pfsm_transitions::TransitionCheck_GoToShield(tank);
            break;
        case tState::repair:
            pfsm_transitions::TransitionCheck_Repair(tank);
            break;
        case tState::damageLookout:
            pfsm_transitions::TransitionCheck_Damage(tank);
            break;
    }

    switch (state) {
        case tState::wander:
            tank.drawable_.set_tint({255, 255, 255});
            break;
        case tState::flee:
            tank.drawable_.set_tint({0, 0, 50});
            break;
        case tState::gotoEMP:
            tank.drawable_.set_tint({0, 0, 150});
            break;
        case tState::gotoShield:
            tank.drawable_.set_tint({0, 0, 255});
            break;
        case tState::repair:
            tank.drawable_.set_tint({10, 10, 10});
            break;
        case tState::damageLookout:
            // no need to tint
            break;
    }
}

void pfsm_transitions::TransitionCheck_Wander(tank& t) {
    t.ufos.clear();
    std::vector<play::actor*> ufos;
    for (auto i = t.begin_perceived(); i != t.end_perceived(); ++i) {
        if (i->EntityType == "ufo") {
            play::actor& ufo = *i;
            ufos.push_back(&ufo);
        }
    }
    if (ufos.size() == 0) return;

    t.ufos = ufos;

    int number;
    {
        float Min = 0;
        float Max = 100;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(Min, Max);
        number = distr(gen);
    }

    if (number >= 100 - t.FleeChance) {
        t.state = tState::flee;
    } else if (number >= 100 - t.FleeChance - t.EMPChance) {
        t.state = tState::gotoEMP;
    } else if (number <= t.ShieldChance) {
        t.state = tState::gotoShield;
    }
}

void pfsm_transitions::TransitionCheck_Repair(tank& t) {
    bool andre_on_edge = false;
    for (auto& edge : t.node()) {
        if (edge.to().node_id() == t.Andre->node().node_id()) {
            andre_on_edge = true;
        }
    }

    if (t.node().node_id() == t.Andre->node().node_id() || andre_on_edge) {
        t.damage = 0;
        t.state = tState::wander;
    }
}

void pfsm_transitions::TransitionCheck_Flee(tank& t) {
    if (t.fleecount >= 10) {
        t.lastchoice = flee;
        t.fleecount = 0;
        t.state = tState::wander;
    }
}

void pfsm_transitions::TransitionCheck_GoToEMP(tank& t) {
    if (t.path.size() == 0 && t.target != nullptr) {
        t.target->NewLocation();
        t.lastchoice = gotoEMP;
        t.EMPCount++;
        t.target = nullptr;

        t.state = tState::wander;
    }
}

void pfsm_transitions::TransitionCheck_GoToShield(tank& t) {
    if (t.path.size() == 0 && t.target != nullptr) {
        t.target->NewLocation();
        t.LaserShieldCount++;
        t.target = nullptr;
        t.lastchoice = gotoShield;

        t.state = tState::wander;
    }
}

void pfsm_transitions::TransitionCheck_Damage(tank& t) {
    if (t.damage >= 100) {
        t.state = tState::repair;
    }
}

#pragma region helpers

#pragma endregion helpers

}  // namespace kmint::ufo::student