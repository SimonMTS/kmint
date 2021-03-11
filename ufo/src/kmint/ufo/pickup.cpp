#include "kmint/ufo/pickup.hpp"


#include "kmint/graphics.hpp"
#include "kmint/map/map.hpp"
#include "kmint/random.hpp"
#include "kmint/ufo/node_algorithm.hpp"
namespace kmint::ufo {

namespace {

graphics::color color_for(ufo::pickup_type type) {
    if (type == pickup_type::SHIELD) {
            return {0, 255, 0};
    } else if (type == pickup_type::EMP) {
    return {0, 0, 255};
    }
    
}


graphics::image image_for(ufo::pickup_type type) {
    //if (type == ufo::pickup_type::SHIELD) return graphics::image{"resources/andre.png"};
    return graphics::image{"resources/andre.png"};
}


}  // namespace


Pickup::Pickup(map::map_graph &g, map::map_node &initial_node, pickup_type type)
    : play::map_bound_actor{initial_node},
      drawable_{*this, graphics::image{image_for(type)}},
      graph_{g},
      node{initial_node} {
    drawable_.set_tint(color_for(type));
    this->type = type;
};

void Pickup::act(delta_time dt) { 
    t_since_move_ += dt;
    if (to_seconds(t_since_move_) >= 1) {
    }
}



}  // namespace kmint::ufo