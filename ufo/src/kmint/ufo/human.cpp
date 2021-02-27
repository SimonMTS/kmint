#include "kmint/ufo/human.hpp"
#include "kmint/random.hpp"
#include <string>

namespace kmint::ufo {

namespace {
constexpr char const *image_path = "resources/human.png";

graphics::image human_image() { return graphics::image{image_path}; }

math::vector2d random_location() {
  return {random_scalar(60, 900), random_scalar(60, 700)};
}

} // namespace
human::human()
	: play::free_roaming_actor{ random_location() },
	drawable_{ *this, human_image() } {
}


} // namespace kmint::ufo
