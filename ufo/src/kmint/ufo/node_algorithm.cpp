#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <algorithm>
#include <cassert>
#include <numeric>

namespace kmint {
namespace ufo {

map::map_node const &random_adjacent_node(map::map_node const &node) {
  int r = random_int(0, node.num_edges());
  return node[r].to();
}

map::map_node const &find_node_of_kind(map::map_graph const &graph, char kind) {
  auto i = std::find_if(graph.begin(), graph.end(), [kind](auto const &node) {
    return node.node_info().kind == kind;
  });
  return *i;
}

map::map_node &random_node_of_kind(map::map &map, char kind) {
  auto b = map.begin_of_kind(kind);
  auto e = map.end_of_kind(kind);
  int n = e - b;
  assert(n > 0);
  return *b[random_int(0, n)];
}
map::map_node const &random_node_of_kind(map::map const &map, char kind) {
  auto b = map.begin_of_kind(kind);
  auto e = map.end_of_kind(kind);
  int n = e - b;
  assert(n > 0);
  return *b[random_int(0, n)];
}

map::map_node const &find_closest_node_to(map::map_graph const &graph,
                                          math::vector2d location) {
  auto i = std::min_element(graph.begin(), graph.end(),
                            [location](auto const &l, auto const &r) {
                              return distance2(l.location(), location) <
                                     distance2(r.location(), location);
                            });
  return *i;
}

double waiting_time(map::map_node const &node) { return node[0].weight(); }

} // namespace ufo
} // namespace kmint
