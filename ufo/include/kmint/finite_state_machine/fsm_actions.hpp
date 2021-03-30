#ifndef STUDENT_FSMA_HPP
#define STUDENT_FSMA_HPP

// #include "../../kmint/ufo/saucer.hpp"
#include "kmint/force_driven_entities/movement_helpers.hpp"
namespace kmint::ufo {
class saucer;
}

#include "kmint/map/map.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {
class fsm_actions {
   public:
    static math::vector2d Execute_Wander(force_driven_entity& s);
    static math::vector2d Execute_HuntHuman(saucer& s);
    static math::vector2d Execute_HuntTank(saucer& s);
    static math::vector2d Execute_NoMove(saucer& s);

    // helpers
    static void BeamHumans(saucer& s);
    static float RandomInt(float Min, float Max);
};
}  // namespace kmint::ufo::student

#endif /* STUDENT_FSMA_HPP */