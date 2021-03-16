#include "kmint/ufo/pickup.hpp"


#include "kmint/graphics.hpp"
#include "kmint/map/map.hpp"
#include "kmint/random.hpp"
#include "kmint/ufo/node_algorithm.hpp"
namespace kmint::ufo {

namespace {



graphics::image image_for(ufo::pickup_type type) {
   if (type == ufo::pickup_type::SHIELD) return graphics::image{"resources/shield.png"};
    return graphics::image{"resources/emp.png"};
}


}  // namespace


Pickup::Pickup(map::map_graph &g, map::map_node &initial_node, pickup_type type)
    : play::map_bound_actor{initial_node},
      drawable_{*this, graphics::image{image_for(type)}},
      graph_{g}
       {
    node(initial_node);
    this->type = type;
};

void Pickup::act(delta_time dt) { 
    t_since_move_ += dt;
    if (to_seconds(t_since_move_) >= 1) {

    }
}

void Pickup::NewLocation() { 
    int id = RandomInt(0, graph_.num_nodes() - 1);
    this->node(graph_[id]);
}


}  // namespace kmint::ufo