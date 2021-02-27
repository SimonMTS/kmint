#include "kmint/math/rectangle.hpp"
#include "gtest/gtest.h"
#include <sstream>

namespace {
using kmint::math::rectangle;

TEST(rectangle, ShouldOutput) {
  rectangle r{{1,2}, {3,4}};
  std::ostringstream os{};
  os << r;
  EXPECT_EQ("1 2 3 4", os.str());
}
TEST(rectangle, ShouldCalculateCenter) {
  rectangle r{{1,2}, {8,4}};
  auto c = center(r);
  kmint::math::vector2d expected{5,4};
  EXPECT_EQ(expected, c);
}

TEST(rectangle, ShouldSplitIntoQuadrants) {
  rectangle r{{0,0}, {4,4}};
  auto q = to_quadrants(r);
  EXPECT_EQ(2, q.upperRight().top_left().x());
  EXPECT_EQ(0, q.upperRight().top_left().y());
  EXPECT_EQ(2, q.upperRight().size().width());
  EXPECT_EQ(2, q.upperRight().size().height());
  EXPECT_EQ(0, q.upperLeft().top_left().x());
  EXPECT_EQ(0, q.upperLeft().top_left().y());
  EXPECT_EQ(2, q.upperLeft().size().width());
  EXPECT_EQ(2, q.upperLeft().size().height());
  EXPECT_EQ(0, q.bottomLeft().top_left().x());
  EXPECT_EQ(2, q.bottomLeft().top_left().y());
  EXPECT_EQ(2, q.bottomLeft().size().width());
  EXPECT_EQ(2, q.bottomLeft().size().height());
  EXPECT_EQ(2, q.bottomRight().top_left().x());
  EXPECT_EQ(2, q.bottomRight().top_left().y());
  EXPECT_EQ(2, q.bottomRight().size().width());
  EXPECT_EQ(2, q.bottomRight().size().height());
}

TEST(rectangle, ShouldSplitIntoQuadrantsAtCompileTime) {
  constexpr rectangle r{{0,0}, {4,4}};
  constexpr auto q = to_quadrants(r);
  EXPECT_EQ(2, q.upperRight().top_left().x());
  EXPECT_EQ(0, q.upperRight().top_left().y());
  EXPECT_EQ(2, q.upperRight().size().width());
  EXPECT_EQ(2, q.upperRight().size().height());
  EXPECT_EQ(0, q.upperLeft().top_left().x());
  EXPECT_EQ(0, q.upperLeft().top_left().y());
  EXPECT_EQ(2, q.upperLeft().size().width());
  EXPECT_EQ(2, q.upperLeft().size().height());
  EXPECT_EQ(0, q.bottomLeft().top_left().x());
  EXPECT_EQ(2, q.bottomLeft().top_left().y());
  EXPECT_EQ(2, q.bottomLeft().size().width());
  EXPECT_EQ(2, q.bottomLeft().size().height());
  EXPECT_EQ(2, q.bottomRight().top_left().x());
  EXPECT_EQ(2, q.bottomRight().top_left().y());
  EXPECT_EQ(2, q.bottomRight().size().width());
  EXPECT_EQ(2, q.bottomRight().size().height());
}

TEST(rectangle, ShouldDetectIntersection) {
  constexpr rectangle r {
    {0, 0}, { 2, 2 }
  };
  constexpr rectangle l {
    {3, 3}, { 3, 3 }
  };
  constexpr rectangle m {
    {1, 1}, { 3, 3 }
  };
  EXPECT_FALSE(intersect(r, l));
  EXPECT_TRUE(intersect(r, m));
}

TEST(rectangle, ContainsWorks) {
  constexpr rectangle r {
    {0, 0}, { 2, 2 }
  };
  EXPECT_TRUE(contains(r, {0, 0}));
  EXPECT_TRUE(contains(r, {2, 2}));
  EXPECT_FALSE(contains(r, {-0.1, -0.1}));
  EXPECT_FALSE(contains(r, {1, 3}));
}

} // namespace
