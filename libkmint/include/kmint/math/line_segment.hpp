#ifndef KMINT_MATH_LINE_SEGMENT_HPP
#define KMINT_MATH_LINE_SEGMENT_HPP

#include "kmint/math/vector2d.hpp"

namespace kmint {
namespace math {

template <typename Scalar> struct basic_line_segment {
  constexpr basic_line_segment() {}
  constexpr basic_line_segment(basic_vector2d<Scalar> const &b,
                               basic_vector2d<Scalar> const &e)
      : begin{b}, end{e} {}
  basic_vector2d<Scalar> begin{};
  basic_vector2d<Scalar> end{};
};
template <typename Scalar>
basic_line_segment(basic_vector2d<Scalar>, basic_vector2d<Scalar>)
    ->basic_line_segment<Scalar>;

using line_segment = basic_line_segment<scalar>;

} // namespace math

} // namespace kmint

#endif /* KMINT_MATH_LINE_SEGMENT_HPP */
