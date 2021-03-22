#ifndef STUDENT_FDEMH_HPP
#define STUDENT_FDEMH_HPP

#include "kmint/map/map.hpp"
#include "kmint/play/free_roaming_actor.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {

class force_driven_entity : public kmint::play::free_roaming_actor {
   public:
    math::vector2d acceleration{};
    math::vector2d WanderDirection;
    math::vector2d velocity{};

    force_driven_entity(math::vector2d loc) : free_roaming_actor(loc) {}

    void set_location(math::vector2d loc) {
        kmint::play::free_roaming_actor::location(loc);
    }
};

class movement_helpers {
   public:
    static void MoveTick(force_driven_entity& a);

    static math::vector2d limit(const kmint::math::vector2d& vector);
};

}  // namespace kmint::ufo::student

#endif /* STUDENT_FDEMH_HPP */