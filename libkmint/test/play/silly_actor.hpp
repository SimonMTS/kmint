#ifndef SILLY_ACTOR_HPP
#define SILLY_ACTOR_HPP

#include "kmint/play/actor.hpp"
#include "kmint/ui/noop_drawable.hpp"
#include <string>

class silly_actor : public kmint::play::actor {
public:
 silly_actor(int i, std::string s)
     : silly_actor{i, s, {0.f, 0.f}} {}
  silly_actor(int i, std::string s, kmint::math::vector2d loc)
      : test_int_{i}, test_string_{std::move(s)}, loc_{loc} {}
  kmint::ui::drawable const &drawable() const override { return drawable_; }
  kmint::math::vector2d location() const override { return loc_; }
  int test_int() const noexcept { return test_int_; }
  std::string const &test_string() const noexcept { return test_string_; }
  bool act_called() const noexcept { return act_called_; }
  void act(kmint::delta_time /*dt*/) override { act_called_ = true; }

private:
  kmint::ui::noop_drawable drawable_{};
  bool act_called_{false};
  int test_int_;
  std::string test_string_;
  kmint::math::vector2d loc_;
};


#endif /* SILLY_ACTOR_HPP */
