#ifndef STUDENT_FDEMH_HPP
#define STUDENT_FDEMH_HPP

#include "kmint/map/map.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/play/free_roaming_actor.hpp"
using kmint::map::map_node;

namespace kmint::ufo::student {

class force_driven_entity : public kmint::play::free_roaming_actor {
   public:
    math::vector2d acceleration{0, 0};
    math::vector2d WanderDirection{0, 0};
    math::vector2d velocity{0, 0};

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
    using forceFunc = std::function<math::vector2d(force_driven_entity&)>;

    static void MoveTick(force_driven_entity& e,
                         std::vector<forceFunc>& forces);

    // forceFunctions
    static math::vector2d AvoidScreenEdge(force_driven_entity& a);

    // helper
    static math::vector2d limit(const kmint::math::vector2d& vector);
};

}  // namespace kmint::ufo::student

#endif /* STUDENT_FDEMH_HPP */