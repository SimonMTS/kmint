#ifndef STUDENT_PFSMA_HPP
#define STUDENT_PFSMA_HPP

namespace kmint::ufo {
class tank;
}

#include "kmint/map/map.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {
class pfsm_actions {
   public:
    static void ExecuteAction(tank& t);

   private:
    static void Execute_Wander(tank& t);
    static void Execute_Repair(tank& t);
    static void Execute_Flee(tank& t);
    static void Execute_GoToEMP(tank& t);
    static void Execute_GoToShield(tank& t);

    // helpers
};
}  // namespace kmint::ufo::student

#endif /* STUDENT_PFSMA_HPP */