#include "kmint/play/stage.hpp"
#include "kmint/play/actor.hpp"
#include "kmint/ui/noop_drawable.hpp"
#include "play/silly_actor.hpp"
#include "gtest/gtest.h"
#include <string>

namespace {
using kmint::play::stage;
constexpr kmint::math::size stage_size {10, 10};

TEST(Stage, ShouldConstruct) {
  stage s{stage_size};
  EXPECT_EQ(true, s.begin() == s.end());
}


TEST(Stage, ItShouldAddActor) {
  stage s{stage_size};
  auto &sa = s.build_actor<silly_actor>(10, "HELLO");
  EXPECT_EQ(&sa, &(*s.begin()));
  EXPECT_EQ(10, sa.test_int());
  EXPECT_EQ("HELLO", sa.test_string());
}

TEST(Stage, ItShouldRemoveActor) {
  stage s{stage_size};
  auto &sa = s.build_actor<silly_actor>(10, "HELLO");
  EXPECT_FALSE(s.cbegin() == s.cend()); // non empty stage
  s.remove_actor(sa);
  EXPECT_TRUE(s.cbegin() == s.cend()); // empty stage
}

TEST(Stage, ItShouldCallUpdate) {
  stage s{stage_size};
  auto &sa = s.build_actor<silly_actor>(10, "HELLO");
  s.act(kmint::from_seconds(1.0));
  EXPECT_TRUE(sa.act_called());
}

} // namespace
