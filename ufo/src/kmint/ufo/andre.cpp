#include "kmint/ufo/andre.hpp"

#include <iostream>

#include "kmint/graphics.hpp"
#include "kmint/map/map.hpp"
#include "kmint/random.hpp"
#include "kmint/ufo/node_algorithm.hpp"

namespace kmint::ufo {

namespace {

graphics::image andre_image() {
    constexpr scalar scale = 1.0;
    return graphics::image{"resources/andre.png"};
}

}  // namespace
andre::andre(map::map_graph &g, map::map_node &initial_node)
    : play::map_bound_actor{initial_node},
      drawable_{*this, graphics::image{andre_image()}},
      graph_{g} {
    EntityType = "andre";
}

void andre::act(delta_time dt) {
    t_since_move_ += dt;
    if (to_seconds(t_since_move_) >= 1) {

        if (this->path.size() <= 1) {
            this->goal_node =
                this->goal_node == '4' ? '1' : this->goal_node + 1;

            graph_.untag_all();
            this->path = student::a_star::find_path(
                node(), find_node_of_kind(graph_, this->goal_node), graph_,
                ufo::student::heuristics::euclidean_distance);
        }

        this->node(this->path[this->path.size() - 2].get());
        this->path.pop_back();
        t_since_move_ = from_seconds(0);
    }
}

}  // namespace kmint::ufo
