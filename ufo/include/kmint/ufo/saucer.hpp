#ifndef KMINT_UFO_SAUCER_HPP
#define KMINT_UFO_SAUCER_HPP
#include "kmint/math/vector2d.hpp"
#include "kmint/play.hpp"
namespace kmint::ufo {
enum class saucer_type { blue, green, beige, yellow };
class saucer : public kmint::play::free_roaming_actor {
public:
  saucer(saucer_type type);
  saucer_type type() const noexcept { return type_; }
  ui::drawable const &drawable() const override { return drawable_; }
  void act(delta_time dt) override;
  // participates in collisions
  bool incorporeal() const override { return false; }
  bool perceptive() const override { return true; }

  scalar collision_range() const override { return 32.0; }
  scalar perception_range() const override { return 1000.0f; }


private:
  play::image_drawable drawable_;
  saucer_type type_;
  math::vector2d velocity{};
  math::vector2d acceleration{};

  delta_time t_since_move_{};

  enum State { hunttank, hunthuman, wander, nomove };
  State state = wander;
  bool TankNearby();
  bool HumanNearby();

  void Wander(delta_time dt);
  void HuntHuman(delta_time dt);
  void HuntTank(delta_time dt);
  void NoMove(delta_time dt);

  void AttackHuman();
  void AttackTank();
  void Move();
  void GetNearest(std::string type);
  void SetWanderDirection();
  void Edges();
  int RandomInt(float Min, float Max);

  math::vector2d WanderDirection;
  play::actor *target;


  math::vector2d NextLocation;
};

} // namespace kmint::ufo
#endif
