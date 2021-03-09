#include "kmint/ufo/tank.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>
#include "kmint/a_star/a_star.hpp"
#include "kmint/a_star/heuristics.hpp"
namespace kmint::ufo {

namespace {
graphics::image tank_image(tank_type t) {
  constexpr scalar scale = 0.35;
  switch (t) {
  case tank_type::red:
    return graphics::image{"resources/tank_red.png", scale};
  case tank_type::green:
    return graphics::image{"resources/tank_green.png", scale};
  }
  return graphics::image{"resources/tank_green.png", scale};
}


} // namespace

tank::tank(map::map_graph& g, map::map_node& initial_node, tank_type t)
    : graph{g},
      play::map_bound_actor{initial_node},
      type_{t},
	drawable_{ *this, graphics::image{tank_image(t)} } {
    EntityType = "tank";
}

void tank::act(delta_time dt) {
    //std::cout << Andre->location() << std::endl;

	t_since_move_ += dt;
	if (to_seconds(t_since_move_) >= 1) {
            graph.untag_all();

            std::cout << "Act" << std::endl;
            switch (state) {
                case wander:
                    Wander();
                    break;
                case flee:
                    Flee();
                    break;
                case gotoEMP:
                    GoToEMP();
                    break; 
                case gotoShield:
                    GoToShield();
                    break;
            }

            Move();
	}
}

void tank::Wander(){ 
    MoveTo(492);
    //next_edge = &node()[random_int(0, node().num_edges())]; 


};
void tank::Flee(){};
void tank::GoToEMP(){};
void tank::GoToShield(){};

void tank::Move() { 
    if (next_edge != last_edge) {
        weight = next_edge->weight() * 5;
        last_edge = next_edge;
    }

    if (weight == 1) {
        node(this->next_edge->to());
    } else {
        weight--;
    }

}

void tank::MoveTo(const int nodeid) {

    const kmint::map::map_node &end = graph[nodeid];
    kmint::map::map_node &start = this->node();
    ufo::student::node_list nodelist  = ufo::student::a_star::find_path(start, end, this->graph, ufo::student::heuristics::euclidean_distance);
    std::cout << nodelist.size() << std::endl;
    //if (nodelist.size() == 1) return;
 /*   std::cout << nodelist.size() << std::endl;
    std::cout << nodelist.front().get().node_id() << std::endl;*/

    if (nodelist.size() > 1) {
        for (int i = 0; i < this->node().num_edges(); i++) {
            if (node()[i].to().node_id() == nodelist.back().get().node_id()) {
                next_edge = &node()[i];
                nodelist.pop_back();
                return;
            }
        }
    } else {
        for (int i = 0; i < this->node().num_edges(); i++) {
            if (node()[i].to().node_id() == nodeid) {
                next_edge = &node()[i];
                return;
            }
        }
    }
    //std::cout << "Moveto" << next_edge << std::endl;
}

} // namespace kmint::ufo
