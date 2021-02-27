#ifndef KMINT_PLAY_GRAPH_BOUND_ACTOR_HPP
#define KMINT_PLAY_GRAPH_BOUND_ACTOR_HPP

#include "kmint/math/vector2d.hpp"
#include "kmint/play/actor.hpp"

namespace kmint::play {

/*! \brief Base class for actors that are situated on a graph node
 * \tparam Graph The type of graph to the nodes of which this actor is bound
 */
template <typename Graph> class graph_bound_actor : public actor {
public:
  using node_type = typename Graph::node_type;
  using edge_type = typename node_type::edge_type;
  //! Construct a graph_bound_actor starting at \a initial_node
  graph_bound_actor(node_type &initial_node)
      : node_{&initial_node} {};
  math::vector2d location() const override { return node_->location(); }

  //! Returns a const reference to the node this actor is currently on
  node_type const &node() const noexcept { return *node_; }
  //! Returns a reference to the node this actor is currently on
  node_type &node() noexcept { return *node_; }
  //! Moves the actor to a new node
  /*!
    This function moves the actor to node \a n.
    \param n the node to move the actor to
  */
  void node(node_type &n) noexcept { node_ = &n; }

private:
  node_type *node_;
};


} // namespace kmint::play

#endif
