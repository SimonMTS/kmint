#ifndef STUDENT_PFSMT_HPP
#define STUDENT_PFSMT_HPP

// #include "../../kmint/ufo/saucer.hpp"
namespace kmint::ufo {
class tank;
}

enum tState { wander, flee, gotoEMP, gotoShield, repair, damageLookout };

#include "kmint/map/map.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {
class pfsm_transitions {
   public:
    static void TransitionCheck(tState state, tank& tank);

   private:
    static void TransitionCheck_Wander(tank& t);
    static void TransitionCheck_Repair(tank& t);
    static void TransitionCheck_Flee(tank& t);
    static void TransitionCheck_GoToEMP(tank& t);
    static void TransitionCheck_GoToShield(tank& t);
    static void TransitionCheck_Damage(tank& t);

    // helpers
};
}  // namespace kmint::ufo::student

#endif /* STUDENT_PFSMT_HPP */