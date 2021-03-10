#ifndef KMINT_UFO_TANK_HPP
#define KMINT_UFO_TANK_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"

#include <random>

namespace kmint {
namespace ufo {

enum class tank_type { red, green };

class tank : public play::map_bound_actor {
public:
  tank(map::map_graph& g, map::map_node& initial_node, tank_type t);
  // wordt elke game tick aangeroepen
  void act(delta_time dt) override;
  ui::drawable const& drawable() const override { return drawable_; }
  // als incorporeal false is, doet de actor mee aan collision detection
  bool incorporeal() const override { return false; }
  // geeft de lengte van een zijde van de collision box van deze actor terug.
  // Belangrijk voor collision detection
  scalar collision_range() const override { return 16.0; }
  // geeft aan dat de tank andere actors kan zien
  bool perceptive() const override { return true; }
  // geeft het bereik aan waarbinnen een tank
  // andere actors kan waarnemen.
  scalar perception_range() const override { return 200.f; }
  play::actor* Andre;

 private:

  enum State { wander, flee, gotoEMP, gotoShield };
  State state = wander;
  void Wander();
  void Flee();
  void GoToEMP();
  void GoToShield();
  void Move();
  void MoveTo(const int nodeid);
  void SenseUFO();
  play::actor* GetNearestUFO(std::vector<play::actor*> ufos);
  void MoveAwayFrom(play::actor* actor);
  void SetSprite();

  int FleeChance = 34;
  int EMPChance = 33;
  int ShieldChance = 33;
  int weight = 0;
  int fleecount = 0;
  map::map_edge* next_edge = nullptr;
  map::map_edge* last_edge = nullptr;
  std::vector<play::actor *> ufos;


  play::image_drawable drawable_;
  delta_time t_since_move_{};
  tank_type type_;
  map::map_graph& graph;

  // Todo dit op een andere plek zetten
   int RandomInt(float Min, float Max) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> distr(Min, Max);
      return distr(gen);
  }
};

} // namespace ufo
} // namespace kmint

#endif /* KMINT_UFO_TANK_HPP */
