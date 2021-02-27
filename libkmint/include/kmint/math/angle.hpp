#ifndef KMINT_MATH_ANGLE_HPP
#define KMINT_MATH_ANGLE_HPP

#include "kmint/primitives.hpp"
#include <cmath>

namespace kmint {
namespace math {

constexpr scalar pi = static_cast<scalar>(3.14159265358979323846);

class angle {
  scalar value_;
  constexpr explicit angle(scalar value) noexcept : value_{value} {}

public:
  constexpr scalar radians() const noexcept { return value_; }
  constexpr scalar degrees() const noexcept {
    return static_cast<scalar>(value_ * 180.0 / pi);
  }
  constexpr angle &operator+=(angle other) noexcept {
    value_ += other.value_;
    return *this;
  }
  constexpr angle &operator-=(angle other) noexcept {
    value_ += other.value_;
    return *this;
  }
  constexpr angle &operator*=(scalar x) noexcept {
    value_ *= x;
    return *this;
  }
  constexpr angle &operator/=(scalar x) noexcept {
    value_ *= x;
    return *this;
  }
  constexpr angle operator-() const noexcept { return angle{-value_}; }
  static constexpr angle from_radians(scalar rad) noexcept {
    return angle{rad};
  }

  static constexpr angle from_degrees(scalar deg) noexcept {
    return angle{deg / 180.f * pi};
  }

  friend constexpr angle operator"" _rad(
      long double r) noexcept; // needs to be
                               // a friend to call private constructor

  friend constexpr angle operator"" _deg(long double d) noexcept; // needs to be
                                                                  // a friend to
                                                                  // call
                                                                  // private
                                                                  // constructor

  friend constexpr angle operator+(angle a, angle b) noexcept { return a += b; }
  friend constexpr angle operator-(angle a, angle b) noexcept { return a -= b; }
  friend constexpr angle operator*(angle a, scalar b) noexcept {
    return a *= b;
  }

  friend constexpr angle operator*(scalar a, angle b) noexcept { return b * a; }

  friend constexpr angle operator/(angle a, scalar b) noexcept {
    return a /= b;
  }

  friend constexpr angle operator/(scalar a, angle b) noexcept { return b / a; }

  friend angle acos(scalar c);
  friend angle asin(scalar c);
  friend angle atan(scalar c);
  friend angle atan2(scalar y, scalar x);
};

constexpr angle operator"" _rad(long double r) noexcept {
  return angle::from_radians(r);
}

constexpr angle operator"" _deg(long double d) noexcept {
  return angle::from_degrees(d);
}

scalar cos(angle a);
scalar sin(angle a);
angle acos(scalar c);
angle atan(scalar c);
angle atan2(scalar y, scalar x);
angle asin(scalar c);

} // namespace math
} // namespace kmint
#endif // KMINT_MATH_ANGLE_HPP
