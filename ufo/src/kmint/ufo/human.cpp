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
        math::vector2d heading = this->heading;
         //todo use a proper merging method here
       //heading += ::student::forces::stay_on_map(*this) * 10;
      /*  heading += ::student::forces::wander(*this);
        heading += ::student::forces::separation(*this);
        heading += ::student::forces::alignment(*this);
        heading += ::student::forces::cohesion(*this);

        heading = ::student::forces::normalize(heading);
        this->heading = heading;

        location(location() + this->heading);*/

        if (this->SafeLocation != nullptr) {
            location(this->SafeLocation->location());
        }
        t_since_move_ = from_seconds(0);
    }
}

}  // namespace kmint::ufo
