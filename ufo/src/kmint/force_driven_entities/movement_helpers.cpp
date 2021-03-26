#include "kmint/force_driven_entities/movement_helpers.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>

#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/ufo/human.hpp"

namespace kmint::ufo::student {

void movement_helpers::MoveTick(force_driven_entity& e,
                                std::vector<forceFunc>& forces) {
    // todo, samenvoeg algo gebruiken
    math::vector2d total_force = {0, 0};
    for (auto& force : forces) {
        total_force += limit(force.first(e)) * force.second;
    }

    e.velocity += limit(total_force);
    e.velocity = limit(e.velocity);

    e.location(e.location() + e.velocity);
    // e.acceleration *= 0;
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
        math::vector2d currPos = a.location();
        math::vector2d futPos = a.location() + (a.velocity * 10);

        // if out of bound, apply opposite force
        if (currPos.x() < 0 || futPos.x() < 0) force += {10, 0};
        if (currPos.x() > 1024 || futPos.x() > 1024) force += {-10, 0};
        if (currPos.y() < 0 || futPos.y() < 0) force += {0, 10};
        if (currPos.y() > 768 || futPos.y() > 768) force += {0, -10};
    }

    return force;
}

math::vector2d movement_helpers::AvoidBuildings(force_driven_entity& a) {
    math::vector2d force = {0, 0};
    math::vector2d currPos = a.location();
    math::vector2d futPos = a.location() + (a.velocity * 10);

    for (const auto& b : a.buildings) {
        if ((futPos.x() > b.TopLeftX && futPos.x() < b.BottomRightX &&
             futPos.y() > b.TopLeftY && futPos.y() < b.BottomRightY) ||
            (currPos.x() > b.TopLeftX && currPos.x() < b.BottomRightX &&
             currPos.y() > b.TopLeftY && currPos.y() < b.BottomRightY)) {
            auto buildingMiddleY =
                b.TopLeftY + ((b.BottomRightY - b.TopLeftY) / 2);
            bool topSide = currPos.y() < buildingMiddleY;
            if (topSide) {
                force += {0, -10};
            } else {
                force += {0, 10};
            }

            auto buildingMiddleX =
                b.TopLeftX + ((b.BottomRightX - b.TopLeftX) / 2);
            bool leftSide = currPos.x() < buildingMiddleX;
            if (leftSide) {
                force += {-10, 0};
            } else {
                force += {10, 0};
            }
        }
    }

    return force;
}

math::vector2d movement_helpers::Separation(force_driven_entity& a) {
    if (a.EntityType != "human") return {0, 0};
    const ufo::human& human = dynamic_cast<ufo::human&>(a);

    kmint::math::vector2d steer{0, 0};
    int count = 0;

    for (unsigned long i = 0; i < human.population->humans.size(); i++) {
        // the memory in other can be corrupted
        kmint::ufo::human& other = *human.population->humans[i];
        // this prevents most crashes, but is not a real fix
        if (!human.removed() && !other.removed() && human.id != other.id &&
            &other.graph_ == &human.graph_) {
            float d = ::student::forces::distance(human, other.location());

            if ((d > 0) && (d < human.DesiredSeparationDistance)) {
                kmint::math::vector2d diff = kmint::math::vector2d{0, 0};
                diff = human.location() - other.location();
                diff = normalize(diff);
                diff = diff / d;
                steer += diff;
                count++;
            }
        }
    }
    if (count > 0) {
        steer = steer / float(count);
    }

    auto steermag = sqrt(steer.x() * steer.x() + steer.y() * steer.y());

    if (steermag > 0) {
        steer = normalize(steer);
        steer *= human.maxSpeed;
        steer -= human.velocity;
        steer = limit(steer, human.maxForce);
    }

    return steer;
}

math::vector2d movement_helpers::Alignment(force_driven_entity& a) {
    if (a.EntityType != "human") return {0, 0};
    const ufo::human& human = dynamic_cast<ufo::human&>(a);

    kmint::math::vector2d sum{0, 0};

    int count = 0;
    for (unsigned long i = 0; i < human.population->humans.size(); i++) {
        // the memory in other can be corrupted
        kmint::ufo::human& other = *human.population->humans[i];
        // this prevents most crashes, but is not a real fix
        if (!human.removed() && !other.removed() && human.id != other.id &&
            &other.graph_ == &human.graph_) {
            float d = ::student::forces::distance(human, other.location());

            if ((d > 0) && (d < human.DesiredAlignmentDistance)) {
                sum += other.velocity;
                count++;
            }
        }
    }

    if (count > 0) {
        sum = sum / (float)count;
        sum = normalize(sum);
        sum = sum * human.maxSpeed;

        kmint::math::vector2d steer;
        steer = sum - human.velocity;
        steer = limit(steer, human.maxForce);
        return steer;
    } else {
        return {0, 0};
    }
}

math::vector2d movement_helpers::Cohesion(force_driven_entity& a) {
    if (a.EntityType != "human") return {0, 0};
    const ufo::human& human = dynamic_cast<ufo::human&>(a);

    kmint::math::vector2d sum = kmint::math::vector2d{0, 0};

    int count = 0;
    for (unsigned long i = 0; i < human.population->humans.size(); i++) {
        // the memory in other can be corrupted
        kmint::ufo::human& other = *human.population->humans[i];
        // this prevents most crashes, but is not a real fix
        if (!human.removed() && !other.removed() && human.id != other.id &&
            &other.graph_ == &human.graph_) {
            float d = ::student::forces::distance(human, other.location());
            if ((d > 0) && (d < human.DesiredCohesionDistance)) {
                sum += other.location();
                count++;
            }
        }
    }

    if (count == 0) {
        return {0, 0};
    }

    sum = sum / count;
    return ::student::forces::seek(human, sum);
}

math::vector2d movement_helpers::GreenTankAttraction(force_driven_entity& a) {
    if (a.EntityType != "human") return {0, 0};
    const ufo::human& human = dynamic_cast<ufo::human&>(a);

    return ::student::forces::attacted_to(human, *human.greentank,
                                          human.DesiredTankDistance);
}

math::vector2d movement_helpers::RedTankAttraction(force_driven_entity& a) {
    if (a.EntityType != "human") return {0, 0};
    const ufo::human& human = dynamic_cast<ufo::human&>(a);

    return ::student::forces::attacted_to(human, *human.redtank,
                                          human.DesiredTankDistance);
}

math::vector2d movement_helpers::UfoAttraction(force_driven_entity& a) {
    if (a.EntityType != "human") return {0, 0};
    const ufo::human& human = dynamic_cast<ufo::human&>(a);

    math::vector2d force{0, 0};
    for (int i = 0; i < human.ufos.size(); i++) {
        force += ::student::forces::attacted_to(human, *human.ufos[i],
                                                human.DesiredUfoDistance);
    }
    return force;
}

math::vector2d movement_helpers::DoorAttraction(force_driven_entity& a) {
    if (a.EntityType != "human") return {0, 0};
    const ufo::human& human = dynamic_cast<ufo::human&>(a);

    math::vector2d force{0, 0};
    for (int i = 0; i < human.doors.size(); i++) {
        force += ::student::forces::attacted_to(human, *human.doors[0], 100);
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

kmint::math::vector2d movement_helpers::limit(const kmint::math::vector2d& v,
                                              float maxforce) {
    kmint::math::vector2d temp = v;
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    if (m > maxforce) {
        temp = {v.x() / m, v.y() / m};
    }
    return temp;
}

kmint::math::vector2d movement_helpers::normalize(
    const kmint::math::vector2d& v) {
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    kmint::math::vector2d temp;
    if (m > 0) {
        temp = kmint::math::vector2d{v.x() / m, v.y() / m};
    } else {
        temp = kmint::math::vector2d{v.x(), v.y()};
    }
    return temp;
}

}  // namespace kmint::ufo::student