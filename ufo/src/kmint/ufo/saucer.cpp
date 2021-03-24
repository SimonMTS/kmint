#include "kmint/ufo/saucer.hpp"

#include <cstdio>
#include <iostream>
#include <queue>
#include <random>
#include <string>

#include "kmint/finite_state_machine/finite_state_machine.hpp"
#include "kmint/finite_state_machine/fsm_actions.hpp"
#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo {

#pragma region helpers

namespace {

char const *color_for(saucer_type type) {
    switch (type) {
        case saucer_type::blue:
            return "Blue";
        case saucer_type::green:
            return "Green";
        case saucer_type::beige:
            return "Beige";
        case saucer_type::yellow:
        default:
            return "Yellow";
    }
}

math::vector2d location_for(saucer_type type) {
    // switch (type) {
    //     case saucer_type::blue:
    //         return {30.f, 30.f};
    //     case saucer_type::green:
    //         return {994.f, 30.f};
    //     case saucer_type::beige:
    //         return {994.f, 738.f};
    //     case saucer_type::yellow:
    //     default:
    //         return {30.f, 738.f};
    // }

    float x = (int)student::fsm_actions::RandomInt(0, 1024);
    float y = (int)student::fsm_actions::RandomInt(0, 768);
    return {x, y};
}

graphics::image image_for(saucer_type type) {
    scalar scale = 0.3;
    std::string prefix{"resources/ship"};
    std::string fn = prefix + color_for(type) + "_manned.png";
    return graphics::image{fn, scale};
}
math::vector2d velocity_for(saucer_type type) {
    switch (type) {
        case saucer_type::blue:
            return {20.f, 20.f};
        case saucer_type::green:
            return {-20.f, 20.f};
        case saucer_type::beige:
            return {-20.f, -20.f};
        case saucer_type::yellow:
        default:
            return {20.f, -20.f};
    }
}

}  // namespace

#pragma endregion helpers

saucer::saucer(saucer_type type, math::vector2d location)
    : student::force_driven_entity{location},
      drawable_{*this, image_for(type)},
      type_{type} {
    EntityType = "ufo";
    velocity = velocity_for(type);

    // SetWanderDirection();
    float xspeed = student::fsm_actions::RandomInt(-3, 3);
    float yspeed = student::fsm_actions::RandomInt(-3, 3);
    WanderDirection = {xspeed, yspeed};
}

void saucer::act(delta_time dt) {
    t_since_move_ += dt;

    if (to_seconds(t_since_move_) >= 0.1) {
        student::finite_state_machine::StateTransitionCheck(*this);
        student::finite_state_machine::ExecuteStateAction(*this);
        BeamHuman();
        t_since_move_ = from_seconds(0);
    }
}

void saucer::BeamHuman() {
    // saucer::AttackHuman() // beam up humans in range
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType != "human") continue;
        play::actor &human = *i;

        ufo::human *human1 = dynamic_cast<ufo::human *>(&human);

        float distance =
            std::sqrt(std::pow(location().x() - human.location().x(), 2) +
                      std::pow(location().y() - human.location().y(), 2));

        if (distance < 20 && !human1->isSafeTank && !human1->isSafeHouse) {
            human1->remove();
        }
    }
}
// void saucer::Move() {
// {  // stay on map (not great, but it works?)
// check position in 100 steps
// math::vector2d futPos = location() + (velocity * 100);

// calulate repulsion
// float xRep =
//     (futPos.x() < 0 ? std::abs(futPos.x()) : futPos.x() - 1024) / 5;
// float yRep =
//     (futPos.y() < 0 ? std::abs(futPos.y()) : futPos.y() - 768) / 5;

// if out of bound, apply opposite force
// if (futPos.x() < 0) WanderDirection += {xRep, 0};
// if (futPos.x() > 1024) WanderDirection += {-xRep, 0};
// if (futPos.y() < 0) WanderDirection += {0, yRep};
// if (futPos.y() > 768) WanderDirection += {0, -yRep};
// }

// velocity += acceleration;

// velocity = limit(velocity);
// math::vector2d nextpos = location() + velocity;
// location(nextpos);
// acceleration *= 0;
// }

#pragma region moreHelpers

void saucer::GetNearest(std::string type) {
    std::priority_queue<std::pair<play::actor *, int>,
                        std::vector<std::pair<play::actor *, int>>,
                        std::function<bool(std::pair<play::actor *, int>,
                                           std::pair<play::actor *, int>)>>
        queue([](std::pair<play::actor *, int> a,
                 std::pair<play::actor *, int> b) {
            return a.second > b.second;
        });

    // TODO netjes boolean van tank en human checken
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType != type) continue;
        play::actor &actor = *i;

        if (i->EntityType == "tank") {
            ufo::tank *tank = dynamic_cast<ufo::tank *>(&actor);
            if (!tank->attackable) continue;
        }

        if (i->EntityType == "human") {
            ufo::human *human = dynamic_cast<ufo::human *>(&actor);
            if (human->isSafeTank) continue;
        }
        float distance =
            std::sqrt(std::pow(location().x() - actor.location().x(), 2) +
                      std::pow(location().y() - actor.location().y(), 2));
        queue.push(std::make_pair(&actor, distance));
    }

    if (!queue.empty()) {
        target = queue.top().first;
    }
}

#pragma endregion moreHelpers

}  // namespace kmint::ufo
