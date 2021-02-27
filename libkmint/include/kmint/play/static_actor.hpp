#ifndef KMINT_PLAY_STATIC_ACTOR_HPP
#define KMINT_PLAY_STATIC_ACTOR_HPP

#include "kmint/math/vector2d.hpp"
#include "kmint/play/actor.hpp"
namespace kmint::play {

/*! \brief Base class for all actors that have a fixed position on the stage
 */
class static_actor : public actor {
public:
  static_actor(math::vector2d fixed_location) : location_{fixed_location} {}
  math::vector2d location() const override { return location_; }

private:
  math::vector2d location_;
};

} // namespace kmint::play

#endif /* KMINT_PLAY_STATIC_ACTOR_HPP */
