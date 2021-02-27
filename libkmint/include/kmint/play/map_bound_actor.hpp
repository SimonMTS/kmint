#ifndef KMINT_PLAY_MAP_BOUND_ACTOR_HPP
#define KMINT_PLAY_MAP_BOUND_ACTOR_HPP

#include "kmint/map/map.hpp"

namespace kmint::play {

//! Base class for actors that are situated on a map.
using map_bound_actor = graph_bound_actor<map::map_graph>;

}  // kmint



#endif /* KMINT_PLAY_MAP_BOUND_ACTOR_HPP */
