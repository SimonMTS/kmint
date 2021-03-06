#ifndef KMINT_UFO_TANK_HPP
#define KMINT_UFO_TANK_HPP

#include <random>

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "kmint/prob_fsm/pfsm_transitions.hpp"
#include "kmint/ufo/andre.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/pickup.hpp"

namespace kmint {

namespace ufo {

enum class tank_type { red, green };
using kmint::map::map_node;
using node_list = std::vector<std::reference_wrapper<map_node>>;

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
    ufo::andre* Andre;
    std::vector<ufo::Pickup*> pickups;

    bool UFOAttack();
    bool attackable = true;

    tState globalState = damageLookout;
    tState state = wander;

    int damage = 0;
    play::image_drawable drawable_;

    std::vector<play::actor*> ufos;

    int FleeChance = 34;
    int EMPChance = 33;
    int ShieldChance = 33;

    int weight = 0;
    int fleecount = 0;
    map::map_edge* next_edge = nullptr;
    map::map_edge* last_edge = nullptr;

    int EMPCount = 0;
    int LaserShieldCount = 0;

    // Kansberekenen
    tState lastchoice = wander;
    std::vector<int> DamageHistory;
    void UpdateChances();

    ufo::Pickup* target = nullptr;
    std::vector<std::reference_wrapper<map_node>> path;

    void MoveAwayFrom(play::actor* actor);
    void GoTo(pickup_type type);
    play::actor* GetNearestUFO(std::vector<play::actor*> ufos);
    void GoToRepair();

   private:
    void TagPath();
    void Wander();
    void Flee();
    void GoToEMP();
    void GoToShield();
    void Repair();
    void Move();
    void MoveTo(const int nodeid);
    void SenseUFO();
    void GetNearestPickup(pickup_type type);
    void SetSprite();
    void RoadkillOrSave();
    bool AvailablePickup(pickup_type type);

    delta_time t_since_move_{};
    delta_time t_since_attack_{};

    tank_type type_;
    map::map_graph& graph;
};

}  // namespace ufo
}  // namespace kmint

#endif /* KMINT_UFO_TANK_HPP */
