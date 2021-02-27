#ifndef KMINT_INTERACTIVE_ACTOR_HPP
#define KMINT_INTERACTIVE_ACTOR_HPP

#include "kmint/play/actor.hpp"
#include "kmint/ui/noop_drawable.hpp"

class interactive_actor : public kmint::play::actor {
public:
  bool is_perceptive{};
  bool is_incorporeal{};
  bool is_perceivable{};
  kmint::scalar p_range{};
  kmint::scalar c_range{};
  kmint::math::vector2d loc{};

  kmint::math::vector2d location() const override { return loc; }
  bool incorporeal() const override { return is_incorporeal; }
  bool perceptive() const override { return is_perceptive; }
  bool perceivable() const override { return is_perceivable; }
  kmint::scalar collision_range() const override { return c_range; }
  kmint::scalar perception_range() const override { return p_range; }

  kmint::ui::drawable const &drawable() const override { return drawable_; }
private:
  kmint::ui::noop_drawable drawable_{};
};




#endif /* KMINT_INTERACTIVE_ACTOR_HPP */
