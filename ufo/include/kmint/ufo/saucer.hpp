#ifndef KMINT_UFO_SAUCER_HPP
#define KMINT_UFO_SAUCER_HPP

#include "kmint/finite_state_machine/finite_state_machine.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/play.hpp"

namespace kmint::ufo {

enum class saucer_type { blue, green, beige, yellow };
enum State { hunttank, hunthuman, wander, nomove };

class saucer : public kmint::play::free_roaming_actor {
   public:
    State state = wander;

    saucer(saucer_type type);

    saucer_type type() const noexcept { return type_; }
    ui::drawable const &drawable() const override { return drawable_; }
    void act(delta_time dt) override;

    // participates in collisions
    bool incorporeal() const override { return false; }
    bool perceptive() const override { return true; }

    scalar collision_range() const override { return 32.0; }
    scalar perception_range() const override { return 200.0f; }

    int seconds_since_move() const { return to_seconds(t_since_move_); }

    play::image_drawable drawable_;
    void GetNearest(std::string type);
    play::actor *target;
    void Move();
    math::vector2d acceleration{};
    math::vector2d WanderDirection;

   private:
    saucer_type type_;
    math::vector2d velocity{};

    delta_time t_since_move_{};

    math::vector2d NextLocation;

    bool TankNearby();
    bool HumanNearby();

    void Wander(delta_time dt);
    void HuntHuman(delta_time dt);
    void HuntTank(delta_time dt);
    void NoMove(delta_time dt);

    void AttackHuman();
    void AttackTank();
    void SetWanderDirection();
    void Edges();
    int RandomInt(float Min, float Max);

    math::vector2d limit(const kmint::math::vector2d &vector);
};

}  // namespace kmint::ufo
#endif
