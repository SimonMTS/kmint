#include "kmint/play/stage.hpp"
#include "kmint/play/actor.hpp"
#include "kmint/ui/noop_drawable.hpp"
#include "play/interactive_actor.hpp"
#include "gtest/gtest.h"

namespace {

TEST(actor, ItShouldPerceivePerceivableActorsInItsRange) {
  interactive_actor ia{};
  ia.is_perceptive = true;
  ia.p_range = 20;
  interactive_actor ib{};
  ib.is_perceivable = true;
  EXPECT_TRUE(sees(ia, ib));
}

TEST(actor, ItShouldntPerceiveInperceivableActorsInItsRange) {
  interactive_actor ia{};
  ia.is_perceptive = true;
  ia.p_range = 20;
  interactive_actor ib{};
  ib.is_perceivable = false;
  EXPECT_FALSE(sees(ia, ib));
}

TEST(actor, ItShouldntPerceivePerceivableActorsOutsideItsRange) {
  interactive_actor ia{};
  ia.is_perceptive = true;
  ia.p_range = 20;
  interactive_actor ib{};
  ib.is_perceivable = true;
  ib.loc = {11.f, 11.f};
  EXPECT_FALSE(sees(ia, ib));
}
TEST(actor, ItShouldCollideWithCorporealActors) {
  interactive_actor ia{};
  ia.is_incorporeal = false;
  ia.c_range = 20;
  interactive_actor ib{};
  ib.is_incorporeal = false;
  ib.c_range = 20;
  ib.loc = {19.f, 19.f};
  EXPECT_TRUE(collides(ia, ib));
}
TEST(actor, ItShouldntCollideWithIncorporealActors) {
  interactive_actor ia{};
  ia.is_incorporeal = false;
  ia.c_range = 20;
  interactive_actor ib{};
  ib.is_incorporeal = true;
  ib.c_range = 20;
  ib.loc = {19.f, 19.f};
  EXPECT_FALSE(collides(ia, ib));
}
TEST(actor, ItShouldntCollideWhenIncorporeal) {
  interactive_actor ia{};
  ia.is_incorporeal = true;
  ia.c_range = 20;
  interactive_actor ib{};
  ib.is_incorporeal = false;
  ib.c_range = 20;
  ib.loc = {19.f, 19.f};
  EXPECT_FALSE(collides(ia, ib));
}


}  // 
