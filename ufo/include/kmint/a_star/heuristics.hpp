#ifndef STUDENT_HEURISTICS_HPP
#define STUDENT_HEURISTICS_HPP

#include "kmint/map/map.hpp"
using kmint::map::map_node;

namespace student {
class heuristics {
   public:
    static int manhattan_distance(map_node &start, const map_node &end);
    static int euclidean_distance(map_node &start, const map_node &end);
    // static int diagonal_distance(map_node &start, const map_node &end);
};
}  // namespace student

#endif /* STUDENT_HEURISTICS_HPP */