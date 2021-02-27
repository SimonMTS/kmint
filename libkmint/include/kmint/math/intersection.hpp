#ifndef KMINT_MATH_INTERSECTION_HPP
#define KMINT_MATH_INTERSECTION_HPP

#include "kmint/math/line_segment.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/primitives.hpp"
#include <iostream>
#include <optional>

namespace kmint {
namespace math {

template <typename Scalar> class basic_intersection {
public:
  using v_type = basic_vector2d<Scalar>;
  v_type point() const { return point_; }
  Scalar distance() const { return distance_; }

  static std::optional<basic_intersection> at(basic_line_segment<Scalar> a,
                                              basic_line_segment<Scalar> b) {
    auto a1 = a.end.x() - a.begin.x();
    auto b1 = b.begin.x() - b.end.x();
    auto c1 = b.begin.x() - a.begin.x();

    auto a2 = a.end.y() - a.begin.y();
    auto b2 = b.begin.y() - b.end.y();
    auto c2 = b.begin.y() - a.begin.y();

    auto denom = a1 * b2 - a2 * b1;

    auto s = (c1 * b2 - c2 * b1) / denom;
    auto t = (a1 * c2 - a2 * c1) / denom;

    if (0 <= s && s <= 1 && 0 <= t && t <= 1) {
      return basic_intersection{{a.begin.x() + a1 * s, a.begin.y() + a2 * s},
                                s};
    }
    return std::nullopt;
  }

private:
  basic_intersection(v_type point, Scalar distance)
      : point_{point}, distance_{distance} {}
  v_type point_;
  Scalar distance_;
};

using intersection = basic_intersection<scalar>;

template <typename Scalar>
std::optional<basic_intersection<Scalar>>
intersect(basic_line_segment<Scalar> a, basic_line_segment<Scalar> b) {
  return basic_intersection<Scalar>::at(a, b);
}

} // namespace math

} // namespace kmint

#endif /* KMINT_MATH_INTERSECTION_HPP */
