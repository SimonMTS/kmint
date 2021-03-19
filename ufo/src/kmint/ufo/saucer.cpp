#include "kmint/ufo/saucer.hpp"

#include <iostream>
#include <string>
#include <random>
#include <queue>

#include "kmint/ufo/human.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/force_driven_entities/forces.hpp"

namespace kmint::ufo {


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
    switch (type) {
        case saucer_type::blue:
            return {30.f, 30.f};
        case saucer_type::green:
            return {994.f, 30.f};
        case saucer_type::beige:
            return {994.f, 738.f};
        case saucer_type::yellow:
        default:
            return {30.f, 738.f};
    }
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

saucer::saucer(saucer_type type)
    : play::free_roaming_actor{location_for(type)},
      drawable_{*this, image_for(type)},
      velocity{velocity_for(type)},
      type_{type} {
    EntityType = "ufo";
    //SetWanderDirection();
}

void saucer::act(delta_time dt) {
    t_since_move_ += dt;

    if (to_seconds(t_since_move_) >= 0.1) {

        switch (state) { 
            case wander:
                Wander(dt);
                break;
            case hunthuman:
                HuntHuman(dt);
                break;
            case hunttank:
                HuntTank(dt);
                break;
            case nomove:
                NoMove(dt);
                break;
        }

        if (state != nomove) {
            Edges();
            Move();
            if (state == hunthuman) {
                AttackHuman();
            }

            if (state == hunttank) {
                AttackTank();
            }
        }

          t_since_move_ = from_seconds(0);
    }
}

bool saucer::TankNearby() {
    for (auto it = begin_perceived(); it != end_perceived(); ++it) {
        if (it->EntityType == "tank") {
            play::actor &tank = *it;
            ufo::tank *tank1 = dynamic_cast<ufo::tank *>(&tank);

            if (tank1->attackable) {
                return true;
            }
        }
      
    }
    return false;
}

bool saucer::HumanNearby() {
    for (auto it = begin_perceived(); it != end_perceived(); ++it) {
        if (it->EntityType == "human") {
            play::actor &human = *it;

            ufo::human *human1 = dynamic_cast<ufo::human *>(&human);
            if (human1->isSafeTank) continue;
         
            return true;
           
        }
    }
    return false;
}

void saucer::NoMove(delta_time dt) { 
    acceleration = {0, 0};
    t_since_move_ += dt; 

    int time = to_seconds(t_since_move_);
    if (time % 2 == 0) {
       drawable_.set_tint({200, 200, 200});
    }
    else {
       drawable_.set_tint({255, 255, 255});

    }


    if (time >= 20) {
        state = wander;
        t_since_move_ = from_seconds(0);
        drawable_.set_tint({255, 255, 255});

    }
}

void saucer::Wander(delta_time dt) {
    if (TankNearby()) {
        HuntTank(dt);
        return;
    }

    if (HumanNearby()) {
        HuntHuman(dt);
        return;
    }

    t_since_move_ += dt;

    //std::cout << "Wander" << std::endl;
    //NextLocation = location() + WanderDirection * 0.01;

    acceleration += WanderDirection * 0.01;
    if (to_seconds(t_since_move_) >= 1) {
        SetWanderDirection();
        t_since_move_ = from_seconds(0);
    }
}

void saucer::HuntHuman(delta_time dt) { 
     if (TankNearby()) {
        HuntTank(dt);
        return;
    }

    state = hunthuman;

    if (!HumanNearby()) {
        Wander(dt);
        return;
    }

    GetNearest("human");
    ufo::human *human = dynamic_cast<ufo::human *>(target);

    math::vector2d desired = target->location() - location(); 
    
    math::vector2d steer = desired + human->velocity;



    steer = ::student::forces::limit(steer, 1);
    acceleration += steer;

}
void saucer::HuntTank(delta_time dt) { 
 state = hunttank;

    if (!TankNearby()) {
        Wander(dt);
        return;
    }

    GetNearest("tank");

    math::vector2d desired = target->location() - location();

    math::vector2d steer = desired;

    steer = ::student::forces::limit(steer, 1);


    acceleration += steer;
}
void saucer::AttackHuman() {
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType != "human") continue;
        play::actor &human = *i;

        ufo::human *human1 = dynamic_cast<ufo::human *>(&human);

        float distance = std::sqrt(std::pow(location().x() - human.location().x(), 2) + std::pow(location().y() - human.location().y(), 2));

        if (distance < 20 && !human1->isSafeTank) {
            human.remove();
        }
        
    }
}
void saucer::AttackTank() {

 for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType != "tank") continue;
        play::actor &tank = *i;

        ufo::tank *tank1 = dynamic_cast<ufo::tank *>(&tank);
        float distance =
            std::sqrt(std::pow(location().x() - tank.location().x(), 2) +
                      std::pow(location().y() - tank.location().y(), 2));
        //  if (distance < eatRange && !pig.isSafe) {

        if (distance < 20 && tank1->attackable) {
            //std::cout << "AttackTank" << std::endl;
            //tank.remove();
            if (tank1->UFOAttack()) {
                state = nomove;
                return;
            };
        }
    }
}
void saucer::Move() { 
    velocity += acceleration;

    velocity = limit(velocity);
    math::vector2d nextpos = location() + velocity;
    location(nextpos);
    acceleration *= 0;
}
void saucer::SetWanderDirection() {
    float xspeed = 20.f * RandomInt(-1, 1);
    float yspeed = 20.f * RandomInt(-1, 1);

    WanderDirection = {xspeed, yspeed};
}

int saucer::RandomInt(float Min, float Max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(-1, 1);
    return distr(gen);
}
bool cmp(std::pair<play::actor *, int> a, std::pair<play::actor *, int> b) {
    if (a.second > b.second) return true;
    return false;
}

void saucer::GetNearest(std::string type) {
    std::priority_queue<std::pair<play::actor *, int>,
                        std::vector<std::pair<play::actor *, int>>,
                        std::function<bool(std::pair<play::actor *, int>,
                                           std::pair<play::actor *, int>)>>
        queue(cmp);

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
            float distance = std::sqrt(std::pow(location().x() - actor.location().x(), 2) + std::pow(location().y() - actor.location().y(), 2));
            queue.push(std::make_pair(&actor, distance));
    
    }

    if (!queue.empty()) {
        target = queue.top().first;
    }
}

math::vector2d saucer::limit(const kmint::math::vector2d &v) {
    kmint::math::vector2d temp = v;
    int maxforce = 3;
    float m = sqrt(v.x() * v.x() + v.y() * v.y());
    if (m > maxforce) {
        temp = {v.x() / m, v.y() / m};
    }
    return temp;
};

void saucer::Edges() {
    kmint::scalar width = 1024;
    kmint::scalar height = 768;

    int edgeboundary = 25;

     // Map borders
    if (location().x() < edgeboundary) {
        if (velocity.x() < 0) {
            acceleration += math::vector2d{-velocity.x() * 2, 0};
        }

    } else if (location().x() > width - edgeboundary) {
        if (velocity.x() > 0) {
            acceleration += math::vector2d{-velocity.x() * 2, 0};
        }
    }

    if (location().y() < edgeboundary) {
        if (velocity.y() < 0) {
            acceleration += math::vector2d{0, -velocity.y() * 2};
        }

    } else if (location().y() > height - edgeboundary) {
        if (velocity.y() > 0) {
           acceleration += math::vector2d{0, -velocity.y() * 2};
        }
    }
}

  }  // namespace kmint::ufo
