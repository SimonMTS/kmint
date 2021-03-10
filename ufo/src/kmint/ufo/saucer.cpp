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
    SetWanderDirection();
}

void saucer::act(delta_time dt) {

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
    //location(location() + v_ * to_seconds(dt));
    //for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
    //    auto &other = colliding_actor(ix);
    //    if (dynamic_cast<human *>(&other)) {
    //        // remove human from play
    //        std::cout << "Beam me up scotty\n";
    //        // -- CHANGE THIS TO MORE SUITABLE BEHAVIOR FOR THE ASSESSMENT
    //        // other.remove();
    //    }
    //}
}

bool saucer::TankNearby() {
    for (auto it = begin_perceived(); it != end_perceived(); ++it) {
        if(it->EntityType == "tank")
    // Veilige mensen niet laten meetellen
      //   if (!it->isSafe) {
            return true;
       // }
    }
    return false;
}

bool saucer::HumanNearby() {
    for (auto it = begin_perceived(); it != end_perceived(); ++it) {
        if (it->EntityType == "human")
            // Veilige mensen niet laten meetellen
            //   if (!it->isSafe) {
            return true;
        // }
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
    state = hunthuman;

    if (!HumanNearby()) {
        Wander(dt);
        return;
    }
    //std::cout << "HuntHuman" << std::endl; 

    GetNearest("human");

    math::vector2d desired = target->location() - location(); 
    
    // TODO Normalise() en Limit()
    // Ufo aanpassen aan de snelheid van de human
    math::vector2d steer = (desired * 0.01);

    acceleration += steer;

}
void saucer::HuntTank(delta_time dt) { 
 state = hunttank;

    if (!TankNearby()) {
        Wander(dt);
        return;
    }
    //std::cout << "HuntTank" << std::endl;

    GetNearest("tank");

    math::vector2d desired = target->location() - location();

    // TODO Normalise() en Limit()
    // Ufo aanpassen aan de snelheid van de human
    math::vector2d steer = (desired * 0.01);

    acceleration += steer;
}
void saucer::AttackHuman() {
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType != "human") continue;
        play::actor &human = *i;

        float distance = std::sqrt(std::pow(location().x() - human.location().x(), 2) + std::pow(location().y() - human.location().y(), 2));
        //  if (distance < eatRange && !pig.isSafe) {

        if (distance < 30) {
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

        if (distance < 30) {
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

    velocity = student::forces::limit(velocity);
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

    // Zet alle humans in de radius in een priority queue en pop de eerste
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        if (i->EntityType != type) continue;
        play::actor &human = *i;
        //if (!pig.isSafe) {
            float distance = std::sqrt(std::pow(location().x() - human.location().x(), 2) + std::pow(location().y() - human.location().y(), 2));
            queue.push(std::make_pair(&human, distance));
       // }
    }

    if (!queue.empty()) {
        target = queue.top().first;
    }
}

void saucer::Edges() {
    kmint::scalar width = 1024;
    kmint::scalar height = 768;
    math::vector2d correction = math::vector2d{0, 0};

    int edgeboundary = 25;

     // Map borders
    if (location().x() < edgeboundary) {
        //std::cout << "Links";
        if (velocity.x() < 0) {
            acceleration += math::vector2d{-velocity.x() * 2, 0};
        }

    } else if (location().x() > width - edgeboundary) {
        //std::cout << "Rechts";
        if (velocity.x() > 0) {
            acceleration += math::vector2d{-velocity.x() * 2, 0};
        }
    }

    if (location().y() < edgeboundary) {
        //std::cout << "Boven" << std::endl;
        if (velocity.y() < 0) {
            acceleration += math::vector2d{0, -velocity.y() * 2};
        }

    } else if (location().y() > height - edgeboundary) {
        //std::cout << "Onder" << std::endl;
        if (velocity.y() > 0) {
           acceleration += math::vector2d{0, -velocity.y() * 2};
        }
    }
    //std::cout << std::endl;
    //if (desired != math::vector2d{0, 0}) {
    //    // desired = Normalize(desired);
    //    //  desired *= 20;
    //    math::vector2d steer = desired - v_;
    //    // steer = Limit(steer, flock->maxForce);

    //    v_ += steer;
    //}












    //// Map borders
    //if (location().x() < edgeboundary) {
    //    desired = math::vector2d{ 20, v_.y()};

    //} else if (location().x() > width - edgeboundary) {
    //    desired = math::vector2d{-20, v_.y()};
    //    //velocity = math::vector2d{0, 0};
    //}

    //if (location().y() < edgeboundary) {
    //    desired = math::vector2d{v_.x(), 20};
    //   // velocity = math::vector2d{0, 0};

    //} else if (location().y() > height - edgeboundary) {
    //    desired = math::vector2d{v_.x(), -20};
    //  //  velocity = math::vector2d{0, 0};
    //}

    //  if (desired != math::vector2d{0, 0}) {
    //   // desired = Normalize(desired);
    //  //  desired *= 20;
    //    math::vector2d steer = desired - v_;
    //   // steer = Limit(steer, flock->maxForce);
    //    
    //    v_ += steer;
    //}
}

  }  // namespace kmint::ufo
