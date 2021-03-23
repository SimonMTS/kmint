#include "kmint/force_driven_entities/movement_helpers.hpp"

#include <cstdio>
#include <iostream>

#include "kmint/math/vector2d.hpp"

namespace kmint::ufo::student {

void movement_helpers::MoveTick(force_driven_entity& e,
                                std::vector<forceFunc>& forces) {
    // todo, samenvoeg algo gebruiken
    math::vector2d total_force = {0, 0};
    for (auto& force : forces) {
        total_force += force(e);
    }
    e.acceleration += total_force;

    e.velocity += e.acceleration;

    e.velocity = limit(e.velocity);
    math::vector2d nextpos = e.location() + e.velocity;
    e.location(nextpos);
    e.acceleration *= 0;
}

math::vector2d movement_helpers::AvoidScreenEdge(force_driven_entity& a) {
    math::vector2d force = {0, 0};

    {
        // stay on map (not great, but it works?)
        // check position in 100 steps
        // math::vector2d futPos = a.location() + (a.velocity * 100);

        // calulate repulsion
        // float xRep =
        //     (futPos.x() < 0 ? std::abs(futPos.x()) : futPos.x() - 1024) / 5;
        // float yRep =
        //     (futPos.y() < 0 ? std::abs(futPos.y()) : futPos.y() - 768) / 5;

        // if out of bound, apply opposite force
        // if (futPos.x() < 0) force += {xRep, 0};
        // if (futPos.x() > 1024) force += {-xRep, 0};
        // if (futPos.y() < 0) force += {0, yRep};
        // if (futPos.y() > 768) force += {0, -yRep};
    }

    {  // stay on map, simple
        // check position in 10 steps
        math::vector2d futPos = a.location() + (a.velocity * 10);

        // if out of bound, apply opposite force
        if (futPos.x() < 0) force += {10, 0};
        if (futPos.x() > 1024) force += {-10, 0};
        if (futPos.y() < 0) force += {0, 10};
        if (futPos.y() > 768) force += {0, -10};
    }

    return force;
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