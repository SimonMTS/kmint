#ifndef KMINT_RANDOM_HPP
#define KMINT_RANDOM_HPP

#include "kmint/math/rectangle.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/primitives.hpp"

namespace kmint {
//! Generate a random integer in the half open range [low, high)
//! \requires high > low
//! \returns a random integer that is larger than or equal to low and less than
//! high
int random_int(int low, int high);
//!
//! Returns a random scalar in the interval [low, high).
//! \requires high > low
//! \returns a random scalar that is larger than or equal to low and less than
//! high.
scalar random_scalar(scalar low, scalar high);

} // namespace kmint
#endif
