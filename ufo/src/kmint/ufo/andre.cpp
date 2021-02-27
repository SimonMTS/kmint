#include "kmint/ufo/andre.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {

namespace {

graphics::image andre_image() {
  constexpr scalar scale = 1.0;
  return graphics::image{"resources/andre.png"};
}

} // namespace
andre::andre(map::map_graph& g, map::map_node& initial_node)
	: play::map_bound_actor{ initial_node }, drawable_{ *this,
													 graphics::image{
														 andre_image()}  }{}

void andre::act(delta_time dt) {
  t_since_move_ += dt;
  if (to_seconds(t_since_move_) >= 1) {
	  // pick random edge
	  int next_index = random_int(0, node().num_edges());
	  this->node(node()[next_index].to());
	  t_since_move_ = from_seconds(0);
  }
}

} // namespace kmint::ufo
