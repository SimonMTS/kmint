#include "kmint/ufo/human.hpp"

#include <string>

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
    buildings.push_back({624, 64, 735, 127});
    buildings.push_back({576, 560, 687, 704});
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
            Forces();
            Buildings();
            MapEdge();
            Move();

            if (isSafeTank) {
                location(greentank->location());
            }
        }
        t_since_move_ = from_seconds(0);
    }
}
void human::Move() {
    if (!isSafeTank) {
        std::vector<student::movement_helpers::forceFunc> inputForces;
        student::movement_helpers::MoveTick(*this, inputForces);
        // acceleration *= 0.4;

        // velocity += acceleration;

        // velocity = ::student::forces::limit(velocity, maxForce);

        // math::vector2d nextpos = location() + velocity;
        // location(nextpos);
        // acceleration *= 0;
    } else {
        location(greentank->location());
    }
}
void human::Forces() {
    if (this->removed()) return;

    math::vector2d s = ::student::forces::separation(*this);
    math::vector2d a = ::student::forces::alignment(*this);
    math::vector2d c = ::student::forces::cohesion(*this);
    math::vector2d greent = ::student::forces::attacted_to(*this, *greentank, DesiredTankDistance);
    math::vector2d redt = ::student::forces::attacted_to(*this, *redtank, DesiredTankDistance);
    
    math::vector2d ufo{0, 0};

    for (int i = 0; i < ufos.size(); i++) {
        ufo +=
            ::student::forces::attacted_to(*this, *ufos[i], DesiredUfoDistance);
    }

    math::vector2d door{0, 0};

    for (int i = 0; i < doors.size(); i++) {
        door += ::student::forces::attacted_to(*this, *doors[0], 100);
    } 

    s = s * SeparationWeight;
    a = a * AlignmentWeight;
    c = c * CohesionWeight;
    greent = greent * GreenTankWeight;
    redt = redt * RedTankWeight;
    ufo = ufo * UfoWeight;
    door = door * DoorWeight;


    acceleration += s + a + c + greent + redt + ufo + door;
}

void human::MapEdge() {
    int TopLeftX = 64;
    int TopLeftY = 0;
    int BottomRightX = 959;
    int BottomRightY = 703;

    int edgeboundary = 5;

    // Map borders
    if (location().x() < TopLeftX + edgeboundary) {
        if (velocity.x() < 0) {
            acceleration += math::vector2d{-velocity.x() * 2, 0};
        }

    } else if (location().x() > BottomRightX - edgeboundary) {
        if (velocity.x() > 0) {
            acceleration += math::vector2d{-velocity.x() * 2, 0};
        }
    }

    if (location().y() < TopLeftY + edgeboundary) {
        if (velocity.y() < 0) {
            acceleration += math::vector2d{0, -velocity.y() * 2};
        }

    } else if (location().y() > BottomRightY - edgeboundary) {
        if (velocity.y() > 0) {
            acceleration += math::vector2d{0, -velocity.y() * 2};
        }
    }
}

void human::Buildings() {
    for (auto b : buildings) {
        if (location().x() > b.TopLeftX && location().x() < b.BottomRightX &&
            location().y() > b.TopLeftY && location().y() < b.BottomRightY) {
            drawable_.set_tint({0, 0, 0});
            acceleration *= -2;
        }
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
