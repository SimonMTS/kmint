#ifndef KMINT_MATH_RECTANGLE_HPP
#define KMINT_MATH_RECTANGLE_HPP

#include "kmint/math/size.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/primitives.hpp"
#include <array>
#include <ostream>

namespace kmint {
namespace math {

template <typename Scalar> class basic_rectangle {
public:
  constexpr basic_rectangle(basic_vector2d<Scalar> top_left,
                            basic_size<Scalar> size)
      : top_left_{top_left}, size_{size} {}
  constexpr basic_vector2d<Scalar> const &top_left() const noexcept {
    return top_left_;
  }
  constexpr basic_size<Scalar> const &size() const noexcept { return size_; }

private:
  basic_vector2d<Scalar> top_left_;
  basic_size<Scalar> size_;
};
using rectangle = basic_rectangle<scalar>;

template <typename Scalar>
constexpr basic_vector2d<Scalar> center(basic_rectangle<Scalar> const &r) {
  return r.top_left() +
         basic_vector2d<Scalar>{r.size().width() / 2, r.size().height() / 2};
}

template <typename Scalar> struct quadrants {
  std::array<basic_rectangle<Scalar>, 4> counterClockwise;
  constexpr const basic_rectangle<Scalar> &upperRight() const noexcept {
    return counterClockwise[0];
  };
  constexpr const basic_rectangle<Scalar> &upperLeft() const noexcept {
    return counterClockwise[1];
  };
  constexpr const basic_rectangle<Scalar> &bottomLeft() const noexcept {
    return counterClockwise[2];
  };
  constexpr const basic_rectangle<Scalar> &bottomRight() const noexcept {
    return counterClockwise[3];
  };
};

template <typename Scalar>
constexpr quadrants<Scalar>
to_quadrants(basic_rectangle<Scalar> const &r) noexcept {
  auto x = r.top_left().x();
  auto y = r.top_left().y();
  auto w = r.size().width();
  auto h = r.size().height();
  auto hw = w / 2;
  auto hh = h / 2;
  auto ohw = w - hw;
  auto ohh = h - hh;

  return quadrants<Scalar>{{{{{x + hw, y}, {ohw, hh}},
                             {{x, y}, {hw, hh}},
                             {{x, y + hh}, {hw, ohh}},
                             {{x + hw, y + hh}, {ohw, ohh}}}}};
}

template <typename Scalar>
constexpr bool contains(basic_rectangle<Scalar> const &r,
                        basic_vector2d<Scalar> const &p) {
  return p.x() >= r.top_left().x() &&
         p.x() <= r.top_left().x() + r.size().width() &&
         p.y() >= r.top_left().y() &&
         p.y() <= r.top_left().y() + r.size().height();
}

template <typename Scalar>
constexpr bool intersect(basic_rectangle<Scalar> const &l,
                         basic_rectangle<Scalar> const &r) noexcept {
  return !(l.top_left().x() > r.top_left().x() + r.size().width() ||
           l.top_left().x() + l.size().width() < r.top_left().x() ||
           l.top_left().y() > r.top_left().y() + r.size().height() ||
           l.top_left().y() + l.size().height() < r.top_left().y());
}

template <typename Scalar>
std::ostream &operator<<(std::ostream &os, basic_rectangle<Scalar> const &br) {
  return os << br.top_left().x() << ' ' << br.top_left().y() << ' '
            << br.size().width() << ' ' << br.size().height();
}

} // namespace math

} // namespace kmint

#endif /* KMINT_MATH_RECTANGLE_HPP */
