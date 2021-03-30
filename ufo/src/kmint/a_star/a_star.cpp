#include "kmint/a_star/a_star.hpp"

#include <kmint/random.hpp>

namespace kmint::ufo::student {

node_list a_star::find_path(map_node &start, const map_node &end,
                            map_graph &graph, const heuristic &heuristic) {
    map<int, student::weights_and_parent> weights;
    for (auto &node : graph) {
        weights[node.node_id()] = {};
        weights[node.node_id()].h = std::numeric_limits<int>::max();
        weights[node.node_id()].f = std::numeric_limits<int>::max();
        weights[node.node_id()].parent = -1;
    }

    // open list
    node_priority_queue open_list(
        [&weights](const map_node &lhs, const map_node &rhs) {
            if (weights[lhs.node_id()].f == weights[rhs.node_id()].f) {
                return weights[lhs.node_id()].h > weights[rhs.node_id()].h;
            } else {
                return weights[lhs.node_id()].f > weights[rhs.node_id()].f;
            }
        });
    weights[start.node_id()].f = 0;
    open_list.push(start);

    // loop over open list
    while (!open_list.empty()) {
        map_node &lowest_f = open_list.top();
        open_list.pop();

        // if reached end
        if (lowest_f.node_id() == end.node_id()) {
            // trace back
            node_list path;

            reference_wrapper<map_node> parent = graph[end.node_id()];
            parent.get().tag(node_tag::path);
            path.push_back(parent);

            while (weights.at(parent.get().node_id()).parent != -1) {
                parent = graph[weights.at(parent.get().node_id()).parent];
                parent.get().tag(node_tag::path);
                path.push_back(parent);
            }

            path.pop_back(); // Huidige locatie verwijderen
            return path;
        }

        for (auto &edge : lowest_f) {
            auto &neighbor = edge.to();

            if (!neighbor.tagged()) {
                neighbor.tag(node_tag::visited);
            }
            int neighbor_weight = edge.weight();
            // for complete avoidance
            // neighbor_weight = (neighbor_weight == 1 ? 1 : 1000);

            int tentative_g = weights[lowest_f.node_id()].f + neighbor_weight;
            int tentative_h = heuristic(neighbor, end);
            int tentative_f = tentative_g + tentative_h;
            if (tentative_f < weights[neighbor.node_id()].f) {
                weights[neighbor.node_id()].h = tentative_h;
                weights[neighbor.node_id()].f = tentative_f;
                weights[neighbor.node_id()].parent = lowest_f.node_id();

                open_list.push(neighbor);
            }
        }
    }

    throw std::runtime_error("no path found");
    return node_list{};
}

}  // namespace kmint::ufo::student
