#include "kmint/play/quad_tree.hpp"
#include "play/silly_actor.hpp"
#include <iterator>
#include <vector>
#include "gtest/gtest.h"

namespace {

using kmint::play::quad_tree;

TEST(quad_tree, ItShouldReturnActors) {
  silly_actor a{1, "aap"};
  silly_actor b{2, "noot", {2.1f, 2.1f}};
  quad_tree<1> qt{kmint::math::rectangle{{0, 0}, {4, 4}}};
  qt.add(a);
  qt.add(b);
  std::vector<kmint::play::actor *> match{};
  qt.find({{2, 2}, {3, 3}}, std::back_inserter(match));
  EXPECT_EQ(match.size(), 1);
  EXPECT_EQ(&b, match[0]);
}

} // namespace
