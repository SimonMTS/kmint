#pragma once

#include "kmint/map/map.hpp"
#include "kmint/primitives.hpp"
#include "kmint/play.hpp"

namespace kmint::ufo {

class Doors : public kmint::play::free_roaming_actor {
   public:
    Doors(map::map_graph &g, math::vector2d location);
    // wordt elke game tick aangeroepen
    void act(delta_time dt) override;
    ui::drawable const &drawable() const override { return drawable_; }

    bool must_draw() const override { return true; }

    // als incorporeal false is, doet de actor mee aan collision detection
    bool incorporeal() const override { return false; }
    // geeft de lengte van een zijde van de collision box van deze actor terug.
    // Belangrijk voor collision detection
    scalar collision_range() const override { return 16.0; }
    // geeft aan dat andr� andere actors kan zien
    bool perceptive() const override { return true; }
    scalar perception_range() const override { return 30.f; }


   private:
    // hoeveel tijd is verstreken sinds de laatste beweging
    delta_time t_since_move_{};
    // weet hoe de koe getekend moet worden
    play::image_drawable drawable_;

    map::map_graph &graph_;
};

}  // namespace kmint::ufo

