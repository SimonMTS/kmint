#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/human.hpp"
#include <string>
#include <iostream>

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

} // namespace

saucer::saucer(saucer_type type)
    : play::free_roaming_actor{location_for(type)},
      drawable_{*this, image_for(type)}, 
	  v_{velocity_for(type)},
	  type_{type} {}

void saucer::act(delta_time dt) {
  location(location() + v_ * to_seconds(dt));
  for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
    auto &other = colliding_actor(ix);
    if (dynamic_cast<human *>(&other)) {
      // remove human from play
	  std::cout << "Beam me up scotty\n";
      // -- CHANGE THIS TO MORE SUITABLE BEHAVIOR FOR THE ASSESSMENT
      other.remove();
    }
  }
}

} // namespace kmint::ufo
