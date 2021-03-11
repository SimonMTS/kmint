#ifndef STUDENT_FORCES_HPP
#define STUDENT_FORCES_HPP

#include <random>

#include "../../ufo/include/kmint/ufo/human.hpp"
#include "../../ufo/include/kmint/ufo/node_algorithm.hpp"
#include "kmint/map/map.hpp"
#include "kmint/math/vector2d.hpp"
using kmint::map::map_node;

namespace student {
class forces {
   public:
    static kmint::math::vector2d stay_on_map(const kmint::ufo::human &human);
    static kmint::math::vector2d wander(const kmint::ufo::human &human);
    static kmint::math::vector2d separation(const kmint::ufo::human &human);
    static kmint::math::vector2d alignment(const kmint::ufo::human &human);
    static kmint::math::vector2d cohesion(const kmint::ufo::human &human);
    static kmint::math::vector2d limit(const kmint::math::vector2d &vector, float maxforce);


   private:
    static float distance(const kmint::ufo::human &human, const kmint::play::actor &actor);
    static kmint::math::vector2d normalize(const kmint::math::vector2d &vector);
    static kmint::math::vector2d seek(const kmint::ufo::human &human, const kmint::math::vector2d &vector);


    static std::default_random_engine generator;
    static std::uniform_real_distribution<float> distribution;
};
}  // namespace student

#endif /* STUDENT_FORCES_HPP */