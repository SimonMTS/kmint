#include "kmint/ufo/saucer.hpp"

#include <iostream>
#include <string>
#include <random>
#include <queue>

#include "kmint/ufo/human.hpp"

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
      v_{velocity_for(type)},
      type_{type} {
    EntityType = "ufo";
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
    }

    Move();

    if (state == hunthuman) {
        AttackHuman();
    }

    if (state == hunttank) {
        AttackTank();
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

void saucer::Wander(delta_time dt) {
    if (TankNearby()) {
        HuntTank(dt);
        return;
    }

    if (HumanNearby()) {
        HuntHuman(dt);
        return;
    }

    //std::cout << "Wander" << std::endl;
    NextLocation = location() + GetRandomVelocity() * to_seconds(dt);

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
    math::vector2d steer = (desired * 0.5);

    NextLocation = location() + steer;

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
    math::vector2d steer = (desired * 0.5);

    NextLocation = location() + steer;

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

        float distance =
            std::sqrt(std::pow(location().x() - tank.location().x(), 2) +
                      std::pow(location().y() - tank.location().y(), 2));
        //  if (distance < eatRange && !pig.isSafe) {

        if (distance < 30) {
            //std::cout << "AttackTank" << std::endl;
            tank.remove();
        }
    }
}
void saucer::Move() { location(NextLocation); }
math::vector2d saucer::GetRandomVelocity() {
    float xspeed = 20.f * RandomInt(-1, 1);
    float yspeed = 20.f * RandomInt(-1, 1);

    return {xspeed, yspeed};
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

  }  // namespace kmint::ufo
