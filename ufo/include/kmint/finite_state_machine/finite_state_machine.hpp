#ifndef STUDENT_FSM_HPP
#define STUDENT_FSM_HPP

// #include "../../kmint/ufo/saucer.hpp"
namespace kmint::ufo {
class saucer;
}

#include "kmint/map/map.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {
enum State { hunttank, hunthuman, wander, nomove, tanklookout };

class finite_state_machine {
   public:
    static void StateTransitionCheck(State state, saucer& saucer);
    static void ExecuteStateAction(saucer& s);
};
}  // namespace kmint::ufo::student

#endif /* STUDENT_FSM_HPP */