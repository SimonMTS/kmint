#ifndef KMINT_PLAY_GRAPH_ACTOR_H
#define KMINT_PLAY_GRAPH_ACTOR_H

#include "kmint/map/map.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/play/free_roaming_actor.hpp"
#include "kmint/play/graph_drawable.hpp"
#include <algorithm>

namespace kmint::play {
/*!
 * \brief A actor that is used to draw a graph on screen
 *
 * A graph actor should not interact with its environment. Its primary
 * purpose is to return a drawable that draws the graph
 *
 * \tparam Graph the type of graph this actor represents
 */
template <typename Graph> class graph_actor : public free_roaming_actor {
public:
  graph_actor(math::vector2d location, Graph const &g)
      : free_roaming_actor{location}, drawable_{g} {};
  bool perceivable() const override { return false; }
  ui::drawable const &drawable() const override { return drawable_; }

private:
  graph_drawable<Graph> drawable_;
};

/*! \brief A specialization of graph_actor for maps
 */
using map_actor = graph_actor<map::map_graph>;
} // namespace kmint::play

#endif
