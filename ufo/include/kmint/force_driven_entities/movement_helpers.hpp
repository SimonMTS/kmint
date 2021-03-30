#ifndef STUDENT_FDEMH_HPP
#define STUDENT_FDEMH_HPP

#include "kmint/map/map.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/play/free_roaming_actor.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {

struct Building {
    int TopLeftX;
    int TopLeftY;
    int BottomRightX;
    int BottomRightY;
};

class force_driven_entity : public kmint::play::free_roaming_actor {
   public:
    math::vector2d acceleration{0, 0};
    math::vector2d WanderDirection{0, 0};
    math::vector2d velocity{0, 0};

    std::vector<Building> buildings;

    force_driven_entity(math::vector2d loc) : free_roaming_actor(loc) {}

    void location(math::vector2d loc) {
        kmint::play::free_roaming_actor::location(loc);
    }
    math::vector2d location() const override {
        return kmint::play::free_roaming_actor::location();
    }
};

class movement_helpers {
   public:
    using forceFunc =
        std::pair<std::function<math::vector2d(force_driven_entity&)>, float>;

    static void MoveTick(force_driven_entity& e,
                         std::vector<forceFunc>& forces);

    // forceFunctions
    static math::vector2d AvoidScreenEdge(force_driven_entity& a);
    static math::vector2d AvoidBuildings(force_driven_entity& a);

    static math::vector2d Separation(force_driven_entity& a);
    static math::vector2d Alignment(force_driven_entity& a);
    static math::vector2d Cohesion(force_driven_entity& a);

    static math::vector2d GreenTankAttraction(force_driven_entity& a);
    static math::vector2d RedTankAttraction(force_driven_entity& a);
    static math::vector2d UfoAttraction(force_driven_entity& a);
    static math::vector2d DoorAttraction(force_driven_entity& a);

    // helper
    static math::vector2d limit(const kmint::math::vector2d& vector);
    static math::vector2d limit(const kmint::math::vector2d& v, float maxforce);
    static kmint::math::vector2d normalize(const kmint::math::vector2d& v);
};

}  // namespace kmint::ufo::student

#endif /* STUDENT_FDEMH_HPP */