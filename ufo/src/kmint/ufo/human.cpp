#include "kmint/ufo/human.hpp"

#include <climits>
#include <string>

#include "kmint/finite_state_machine/fsm_actions.hpp"
#include "kmint/force_driven_entities/forces.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/random.hpp"
#include "kmint/ufo/node_algorithm.hpp"

namespace kmint::ufo {

namespace {
constexpr char const *image_path = "resources/human.png";

graphics::image human_image() { return graphics::image{image_path}; }

math::vector2d random_location() {
    math::vector2d loc = {random_scalar(60, 900), random_scalar(60, 700)};

    return loc;
}

}  // namespace
human::human(map::map_graph &g, const int id)
    : student::force_driven_entity{random_location()},
      graph_{g},
      drawable_{*this, human_image()},
      id{id} {
    EntityType = "human";
    CohesionWeight = RandomNumber(0, 1);
    SeparationWeight = RandomNumber(0, 1);
    AlignmentWeight = RandomNumber(0, 1);
    RedTankWeight = RandomNumber(-1, 1);
    GreenTankWeight = RandomNumber(-1, 1);
    UfoWeight = RandomNumber(-1, 1);
    DoorWeight = RandomNumber(-1, 1);
    buildings.push_back({320, 512, 431, 639});
    buildings.push_back({432, 464, 511, 623});
    buildings.push_back({576, 400, 687, 495});
    buildings.push_back({576, 208, 671, 319});
    buildings.push_back({576, 64, 623, 143});
    buildings.push_back({622, 64, 735, 127});
    // buildings.push_back({576, 560, 687, 704}); // hier mogen ze vgm wel
    // lopen?
    location(CanSpawnHere(location()));
}

void human::act(delta_time dt) {
    t_since_move_ += dt;
    if (to_seconds(t_since_move_) >= 0.1) {
        TimeAlive += to_seconds(t_since_move_);
        DistanceTravelled +=
            sqrt((velocity.x() * velocity.x()) + velocity.y() * velocity.y()) /
            10;

        population->humanproperties[id].Fitness = TimeAlive + DistanceTravelled;
        // std::cout << id << " " << population->humanproperties[id].Fitness <<
        // std::endl;

        if (!isSafeHouse) {
            std::vector<movement_helpers::forceFunc> inputForces{
                {movement_helpers::AvoidBuildings, INT_MAX},
                {movement_helpers::AvoidScreenEdge, INT_MAX},
                {movement_helpers::Separation, SeparationWeight},
                {movement_helpers::Alignment, AlignmentWeight},
                {movement_helpers::Cohesion, CohesionWeight},
                {movement_helpers::GreenTankAttraction, GreenTankWeight},
                {movement_helpers::RedTankAttraction, RedTankWeight},
                {movement_helpers::UfoAttraction, UfoWeight},
                {movement_helpers::DoorAttraction, DoorWeight},
                {student::fsm_actions::Execute_Wander, 1}};
            movement_helpers::MoveTick(*this, inputForces);

            if (isSafeTank) {
                location(greentank->location());
            }
        }
        t_since_move_ = from_seconds(0);
    }
}

math::vector2d human::CanSpawnHere(math::vector2d location) {
    for (auto b : buildings) {
        if (location.x() > b.TopLeftX && location.x() < b.BottomRightX &&
            location.y() > b.TopLeftY && location.y() < b.BottomRightY) {
            return CanSpawnHere(random_location());
        }
    }

    return location;
}

}  // namespace kmint::ufo
