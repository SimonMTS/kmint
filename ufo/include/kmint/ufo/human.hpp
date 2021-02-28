#ifndef KMINT_UFO_HUMAN_HPP
#define KMINT_UFO_HUMAN_HPP

#include <tuple>
#include <vector>

#include "kmint/math/vector2d.hpp"
#include "kmint/play.hpp"

namespace kmint::ufo {

class human : public kmint::play::free_roaming_actor {
   public:
    human(map::map_graph &g);
    const ui::drawable &drawable() const override { return drawable_; }
    // participates in collisions
    bool incorporeal() const override { return false; }
    void act(delta_time dt) override;

    bool perceptive() const override { return true; }
    scalar perception_range() const override { return 30.0f; }
    map::map_graph &graph_;
    math::vector2d heading{1, 0};
    std::vector<std::reference_wrapper<ufo::human>> *other_humans;

   private:
    play::image_drawable drawable_;
    delta_time t_since_move_{};
};

}  // namespace kmint::ufo

#endif /* KMINT_UFO_HUMAN_HPP */
