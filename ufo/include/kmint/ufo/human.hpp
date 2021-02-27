#ifndef KMINT_UFO_HUMAN_HPP
#define KMINT_UFO_HUMAN_HPP

#include "kmint/play.hpp"
#include <tuple>
#include <vector>

namespace kmint::ufo {

class human : public kmint::play::free_roaming_actor {
public:
  human();
  const ui::drawable& drawable() const override { return drawable_; }
  // participates in collisions
  bool incorporeal() const override { return false; }


private:
  play::image_drawable drawable_;
};

} // namespace kmint::ufo

#endif /* KMINT_UFO_HUMAN_HPP */
