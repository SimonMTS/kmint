#ifndef KMINT_GRAPH_BASIC_EDGE_HPP
#define KMINT_GRAPH_BASIC_EDGE_HPP

#include "kmint/math/vector2d.hpp"

namespace kmint {
namespace graph {
template <typename NodeInfo> class basic_node;

//! Represents a weighted directed edge
/*!
  This class represents a directed edge in a node. Directed edges have
  a direction. That is, they can be seen as a one way street from a starting
  point (\sa basic_edge::from) to a destination (\sa basic_edge::to).
  
  \c basic_edge maintains references to its starting point and destination
  and consequently, does not own any nodes.
 */
template <typename NodeInfo> class basic_edge {
public:
  using node_type = basic_node<NodeInfo>;
  //! Construct a directed edge
  /*! 
    Constructs a directed edge from \a from to \a to. 

    \param from the starting point of the edge
    \param to the destination of the edge
    \param weight an optional weight (cost) associated with the edge (defaults to \a 1.0)
   */
  basic_edge(node_type &from, node_type &to, float weight = 1.0f)
      : from_{&from}, to_{&to}, weight_{weight} {}
  //! Returns a const reference to the destination node
  node_type const &to() const noexcept { return *to_; }
  //! Returns a reference to the destination node
  node_type &to() noexcept { return *to_; }
  //! Returns a const reference to the starting node
  node_type const &from() const noexcept { return *from_; }
  //! Returns a reference to the starting node
  node_type &from() noexcept { return *from_; }
  //! Returns the weight associated with this edge
  float weight() const noexcept { return weight_; }

private:
  node_type *from_;
  node_type *to_;
  float weight_;
};

} // namespace graph
} // namespace kmint

#endif
