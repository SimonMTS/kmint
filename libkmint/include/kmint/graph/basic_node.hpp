#ifndef KMINT_GRAPH_BASIC_NODE_HPP
#define KMINT_GRAPH_BASIC_NODE_HPP

#include "kmint/graph/basic_edge.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/util/deref_unique_ptr.hpp"
#include "kmint/util/wrapped_iterator.hpp"
#include <memory>
#include <type_traits>
#include <vector>

namespace kmint {
namespace graph {

/*! A tag used to indicate whether the note belongs was visited or is part of a
 * path
 */
enum class node_tag {
  normal,  /**< default tag for a node */
  visited, /**< indicates the node belongs to the closed set of a path finding
              algoritm */
  path     /**< indicates a node belongs to a path */
};
template <typename NodeInfo> class basic_graph;

//! A node in a graph

/// node represents a node in a graph. Every node has an associated information
/// object. This can be an arbitrary class.
/// \requires `NodeInfo`
template <typename NodeInfo> class basic_node {

public:
  using info_type = NodeInfo;
  //! The associated edge type for this node
  using edge_type = basic_edge<NodeInfo>;
  using container = std::vector<std::unique_ptr<edge_type>>;
  using iterator =
      kmint::util::wrapped_iterator<edge_type, typename container::iterator>;
  using const_iterator =
      kmint::util::wrapped_iterator<const edge_type,
                                    typename container::const_iterator>;
  basic_node(basic_node const &) = delete;
  basic_node(basic_node &&) noexcept = default;
  basic_node &operator=(basic_node const &) = delete;
  basic_node &operator=(basic_node &&) noexcept = default;
  ~basic_node() = default;

  NodeInfo &node_info() noexcept { return node_info_; }
  NodeInfo const &node_info() const noexcept { return node_info_; }

  kmint::math::vector2d location() const noexcept { return location_; }
  void add_edge(basic_node &to, float weight) {
    edges_.push_back(std::make_unique<edge_type>(*this, to, weight));
  }
  const_iterator begin() const noexcept {
    return {edges_.begin(), util::deref_unique_ptr_const<edge_type>};
  }
  const_iterator end() const noexcept {
    return {edges_.end(), util::deref_unique_ptr_const<edge_type>};
  }
  const_iterator cbegin() const noexcept {
    return {edges_.begin(), util::deref_unique_ptr_const<edge_type>};
  }
  const_iterator cend() const noexcept {
    return {edges_.end(), util::deref_unique_ptr_const<edge_type>};
  }
  iterator begin() noexcept {
    return {edges_.begin(), util::deref_unique_ptr<edge_type>};
  }
  iterator end() noexcept {
    return {edges_.end(), util::deref_unique_ptr<edge_type>};
  }
  edge_type const &operator[](std::size_t index) const noexcept {
    return *edges_[index];
  }
  edge_type &operator[](std::size_t index) noexcept { return *edges_[index]; }
  std::size_t num_edges() const noexcept { return edges_.size(); }
  std::size_t node_id() const noexcept { return node_id_; }
  /*!
   * Returns whether this node was tagged
   *
   * A node is considered to be tagged if its tag is unequal to node_tag::normal
   *
   * \return Whether the node is tagged or not
   */
  [[deprecated]] bool tagged() const noexcept {
    return tag_ != node_tag::normal;
  }
  /*!
   * Sets the tagged state of a node
   *
   * This function is provided solely for backwards compatibility reasons.
   * Passing false will set this node's tag to node_tag::normal. Passing true
   * will set this node's tag to node_tag::visisted
   *
   */
  [[deprecated]] void tagged(bool t) noexcept {
    tag_ = t ? node_tag::visited : node_tag::normal;
  }
  /*!
   * \return this node's tag
   */
  node_tag tag() const noexcept { return tag_; };
  /*!
   * Changes this node's tag.
   * \param t the tag to assign to this node.
   */
  void tag(node_tag t) noexcept { tag_ = t; };

  friend class basic_graph<NodeInfo>;

private:
  explicit basic_node(std::size_t id, math::vector2d loc) noexcept(
      std::is_nothrow_default_constructible<NodeInfo>::value)
      : node_id_{id}, location_{loc} {};
  basic_node(std::size_t id, math::vector2d loc, NodeInfo const &info) noexcept(
      std::is_nothrow_copy_constructible<NodeInfo>::value)
      : node_id_{id}, location_{loc}, node_info_{info} {};
  basic_node(std::size_t id, math::vector2d loc, NodeInfo &&info) noexcept(
      std::is_nothrow_move_constructible<NodeInfo>::value)
      : node_id_{id}, location_{loc}, node_info_{std::move(info)} {};

  std::size_t node_id_;
  kmint::math::vector2d location_;
  NodeInfo node_info_;
  node_tag tag_{node_tag::normal};
  container edges_{};
};

} // namespace graph
} // namespace kmint

#endif
