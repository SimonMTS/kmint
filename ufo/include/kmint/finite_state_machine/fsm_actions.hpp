#ifndef STUDENT_FSMA_HPP
#define STUDENT_FSMA_HPP

// #include "../../kmint/ufo/saucer.hpp"
namespace kmint::ufo {
class saucer;
}

#include "kmint/map/map.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {
class fsm_actions {
   public:
    static void Execute_Wander(saucer& s);
    static void Execute_HuntHuman(saucer& s);
    static void Execute_HuntTank(saucer& s);
    static void Execute_NoMove(saucer& s);

   private:
    // helpers
    static int RandomInt(float Min, float Max);
};
}  // namespace kmint::ufo::student

#endif /* STUDENT_FSMA_HPP */