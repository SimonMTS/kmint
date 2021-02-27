#ifndef UFO_NODE_ALGORITHM_HPP
#define UFO_NODE_ALGORITHM_HPP

#include "kmint/map/map.hpp"
namespace kmint {
namespace ufo {

///
/// Gets the number of periods for which an entity has to wait before
/// it can move to an adjacent node
///
double waiting_time(map::map_node const &node);

///
/// Finds a random node adjacent to argument node
///
map::map_node const &random_adjacent_node(map::map_node const &node);

///
/// Finds a node of a given kind (which is the character by which it is
/// represented textually)
///
/// You must take care that a node of the given kind exists. Otherwise the
/// behaviour of this function is undefined.
///
map::map_node const &find_node_of_kind(map::map_graph const &graph, char kind);

map::map_node &random_node_of_kind(map::map &map, char kind);
map::map_node const &random_node_of_kind(map::map const &map, char kind);

///
/// Finds the closest node to a location
///
map::map_node const &find_closest_node_to(map::map_graph const &graph,
                                          math::vector2d location);

} // namespace ufo
} // namespace kmint

#endif /* UFO_NODE_ALGORITHM_HPP */
