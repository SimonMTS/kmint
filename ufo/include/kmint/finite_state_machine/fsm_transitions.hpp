#ifndef STUDENT_FSMT_HPP
#define STUDENT_FSMT_HPP

// #include "../../kmint/ufo/saucer.hpp"
namespace kmint::ufo {
class saucer;
}

#include "kmint/map/map.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {
class fsm_transitions {
   public:
    static void TransitionCheck_Wander(saucer& s);
    static void TransitionCheck_HuntHuman(saucer& s);
    static void TransitionCheck_HuntTank(saucer& s);
    static void TransitionCheck_NoMove(saucer& s);
    static void TransitionCheck_TankLookout(saucer& s);

   private:
    // helpers
    static bool TankNearby(saucer& s);
    static bool HumanNearby(saucer& s);
};
}  // namespace kmint::ufo::student

#endif /* STUDENT_FSMT_HPP */