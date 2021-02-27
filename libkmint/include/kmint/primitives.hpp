#ifndef KMINT_PRIMITIVES_HPP
#define KMINT_PRIMITIVES_HPP

#include <chrono>
/*! \defgroup Time functions related to time in the game
@{
*/
namespace kmint {
using scalar = float;

/*! \brief Defines the timer that is used for calculating ticks in game
  \ingroup Time
 */
using timer = std::chrono::steady_clock;
/*! \brief Defines a time point 
  \ingroup Time
 */
using time = timer::time_point;
/*! \brief The difference between two time points 
  \ingroup Time
 */
using delta_time = timer::duration;

/*! \brief Returns the current time point!
  \return The current time point
  \ingroup Time
 */
inline time now() { return timer::now(); }

/*! \brief Converts a delta_time into a number of seconds

  \param dt The time delta to convert
  \return The number of seconds (including a decimal part)
  \ingroup Time
 */
inline float to_seconds(delta_time dt) {
  return std::chrono::duration_cast<std::chrono::duration<float>>(dt).count();
}

/*! \brief Converts a duration in seconds to a delta_time

  \param sec The number of seconds (may include a decimal part)
  \return The time interval represented as a delta_time object
  \ingroup Time
 */
inline delta_time from_seconds(float sec) {
  return std::chrono::duration_cast<delta_time>(
      std::chrono::duration<float>(sec));
}

} // namespace kmint
/*! @} */

#endif /* KMINT_PRIMITIVES_HPP */
