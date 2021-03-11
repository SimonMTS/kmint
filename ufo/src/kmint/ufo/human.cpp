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
    return {random_scalar(60, 900), random_scalar(60, 700)};
}

}  // namespace
human::human(map::map_graph &g)
    : play::free_roaming_actor{random_location()},
      graph_{g},
      drawable_{*this, human_image()} {
    EntityType = "human";
}

void human::act(delta_time dt) {
    t_since_move_ += dt;
    if (to_seconds(t_since_move_) >= 0.1) {

        Forces();
        Move();

        if (this->isSafeTank) {
            location(greentank->location());
        }

        // if this->isSafeBuilding
        t_since_move_ = from_seconds(0);
    }
}
void human::Move() {
    if (!isSafeTank) {
        acceleration *= 0.4;

        velocity += acceleration;

        velocity = ::student::forces::limit(velocity, maxForce);

        math::vector2d nextpos = location() + velocity;
        location(nextpos);
        acceleration *= 0;
    } else {
        location(greentank->location());

    }
}
void human::Forces() {

    math::vector2d s = ::student::forces::separation(*this);
    math::vector2d a = ::student::forces::alignment(*this);
    math::vector2d c = ::student::forces::cohesion(*this);
    math::vector2d greent = ::student::forces::attacted_to(*this, *greentank, true);
    math::vector2d redt = ::student::forces::attacted_to(*this, *redtank, true);
    math::vector2d ufo0 = ::student::forces::attacted_to(*this, *ufos[0], true);
    math::vector2d ufo1 = ::student::forces::attacted_to(*this, *ufos[1], true);
    math::vector2d ufo2 = ::student::forces::attacted_to(*this, *ufos[2], true);
    math::vector2d ufo3 = ::student::forces::attacted_to(*this, *ufos[3], true);

    //math::vector2d b = BoatSeparation();
   // math::vector2d p = PredatorSeparation();

    //s = s * SeparationWeight;
    //a = a * AlignmentWeight;
    //c = c * CohesionWeight;
    //b = b * BoatWeight;
    //p = p * PredatorWeight;

    acceleration += ufo0 + (ufo1 * -1) + ufo2 + (ufo3 * -1);
   // acceleration += s + a + c;
}

}  // namespace kmint::ufo
