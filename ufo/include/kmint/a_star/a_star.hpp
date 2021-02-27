#ifndef STUDENT_A_STAR_HPP
#define STUDENT_A_STAR_HPP

#include <exception>
#include <limits>
#include <map>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

#include "kmint/map/map.hpp"

namespace kmint::ufo::student {
using kmint::graph::node_tag;
using kmint::map::map_graph;
using kmint::map::map_node;
using std::map;
using std::pair;
using std::reference_wrapper;
using std::vector;
using heuristic = int (*)(map_node &, const map_node &);
using node_list = std::vector<std::reference_wrapper<map_node>>;
using node_priority_queue = std::priority_queue<
    std::reference_wrapper<map_node>, node_list,
    std::function<bool(const map_node &, const map_node &)>>;

struct weights_and_parent {
    int g, h, f, parent;
    weights_and_parent() : g(0), h(0), f(0), parent(-1) {}
    weights_and_parent(int g_, int h_, int f_, int parent_)
        : g(g_), h(h_), f(f_), parent(parent_) {}
};

class a_star {
   public:
    static node_list find_path(map_node &start, const map_node &end,
                               map_graph &graph, const heuristic &heuristic);
};
}  // namespace kmint::ufo::student

#endif /* STUDENT_A_STAR_HPP */