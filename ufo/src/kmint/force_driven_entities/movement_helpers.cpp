#include "kmint/force_driven_entities/movement_helpers.hpp"

namespace kmint::ufo::student {

void movement_helpers::MoveTick(force_driven_entity& e) {
    {  // stay on map (not great, but it works?)
        // check position in 100 steps
        math::vector2d futPos = e.location() + (e.velocity * 100);

        // calulate repulsion
        float xRep =
            (futPos.x() < 0 ? std::abs(futPos.x()) : futPos.x() - 1024) / 5;
        float yRep =
            (futPos.y() < 0 ? std::abs(futPos.y()) : futPos.y() - 768) / 5;

        // if out of bound, apply opposite force
        if (futPos.x() < 0) e.WanderDirection += {xRep, 0};
        if (futPos.x() > 1024) e.WanderDirection += {-xRep, 0};
        if (futPos.y() < 0) e.WanderDirection += {0, yRep};
        if (futPos.y() > 768) e.WanderDirection += {0, -yRep};
    }

    e.velocity += e.acceleration;

    e.velocity = limit(e.velocity);
    math::vector2d nextpos = e.location() + e.velocity;
    e.set_location(nextpos);
    e.acceleration *= 0;
}

math::vector2d movement_helpers::limit(const kmint::math::vector2d& v) {
    kmint::math::vector2d temp = v;
    int maxforce = 3;
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    if (m > maxforce) {
        temp = {v.x() / m, v.y() / m};
    }
    return temp;
}

}  // namespace kmint::ufo::student