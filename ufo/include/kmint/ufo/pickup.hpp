#ifndef KMINT_UFO_PICKUP_HPP
#define KMINT_UFO_PICKUP_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include <random>

namespace kmint::ufo {

enum class pickup_type { EMP, SHIELD};
class Pickup : public play::map_bound_actor {
   public:
    Pickup(map::map_graph &g, map::map_node &initial_node, pickup_type type);
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
    scalar perception_range() const override { return 200.f; }

    pickup_type type;
    bool available = true;

    void NewLocation();
    private:
    // hoeveel tijd is verstreken sinds de laatste beweging
    delta_time t_since_move_{};
    // weet hoe de koe getekend moet worden
    play::image_drawable drawable_;

    map::map_graph &graph_;

    int RandomInt(float Min, float Max) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> distr(Min, Max);
      return distr(gen);
  }
};

}  // namespace kmint::ufo

#endif /* KMINT_UFO_ANDRE_HPP */
