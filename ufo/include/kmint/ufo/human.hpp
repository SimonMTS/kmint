#ifndef KMINT_UFO_HUMAN_HPP
#define KMINT_UFO_HUMAN_HPP

#include <tuple>
#include <vector>

#include "kmint/math/vector2d.hpp"
#include "kmint/play.hpp"

class Population;
struct Building {
    int TopLeftX;
    int TopLeftY;
    int BottomRightX;
    int BottomRightY;
};

namespace kmint::ufo {

class human : public kmint::play::free_roaming_actor {
   public:
    human(map::map_graph &g, const int id);
    const ui::drawable &drawable() const override { return drawable_; }
    // participates in collisions
    bool incorporeal() const override { return false; }
    void act(delta_time dt) override;

    bool perceptive() const override { return true; }
    scalar perception_range() const override { return 30.0f; }
    map::map_graph &graph_;
    math::vector2d heading{1, 0};
    // std::vector<std::reference_wrapper<ufo::human>> *other_humans;
    bool isSafeTank = false;
    bool isSafeHouse = false;

    std::vector<play::actor *> ufos;
    std::vector<play::actor *> doors;
    play::actor *greentank;
    play::actor *redtank;

    const float maxSpeed = 4;
    const float maxForce = 1;

    const float DesiredTankDistance = 100;
    const float DesiredUfoDistance = 200;

    const float DesiredSeparationDistance = 100;
    const float DesiredAlignmentDistance = 100;
    const float DesiredCohesionDistance = 100;

    math::vector2d acceleration{0, 0};
    math::vector2d velocity{0, 0};

    Population *population = nullptr;

    float CohesionWeight;    //    0 tot 1
    float SeparationWeight;  //    0 tot 1
    float AlignmentWeight;   //	   0 tot 1
    float RedTankWeight;     //   -1 tot 1;
    float GreenTankWeight;   //   -1 tot 1;
    float UfoWeight;         //   -1 tot 1;
    float DoorWeight;        //   -1 tot 1;

    void setLocation(math::vector2d location);

    // To calculate fitness
    const int id;
    float DistanceTravelled = 0;
    float TimeAlive = 0;

   private:
    std::vector<Building> buildings;

    void Forces();
    void Move();
    void Buildings();
    void MapEdge();
    math::vector2d CanSpawnHere(math::vector2d location);
    play::image_drawable drawable_;
    delta_time t_since_move_{};

    float RandomNumber(float Min, float Max) {
        return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    }
};

}  // namespace kmint::ufo

#endif /* KMINT_UFO_HUMAN_HPP */
