#ifndef KMINT_PLAY_STAGE_HPP
#define KMINT_PLAY_STAGE_HPP

#include "kmint/math/rectangle.hpp"
#include "kmint/play/actor.hpp"
#include "kmint/play/quad_tree.hpp"
#include "kmint/primitives.hpp"
#include "kmint/util/deref_unique_ptr.hpp"
#include "kmint/util/wrapped_iterator.hpp"
#include <memory>
#include <optional>
#include <type_traits>
#include <vector>

namespace kmint {
namespace play {

class stage_overlay;

/*! \brief A stage manages actors
 *
 * The stage facilitates the interaction of actors in a simulation.
 *
 * It constructs and owns the actors on stage. It is responsible for maintaining
 * the actors' perceived and collision sets and calls the actors' act methods
 * every tick.
 */
class stage {
  using container = std::vector<std::unique_ptr<actor>>;

public:
  using iterator = kmint::util::wrapped_iterator<actor, container::iterator>;
  using const_iterator =
      kmint::util::wrapped_iterator<const actor, container::const_iterator>;
  stage(math::size size);
  ~stage();
  stage(stage &&) = default;
  stage(stage const &) = delete;

  stage &operator=(stage const &) = delete;
  stage &operator=(stage &&) = default;

  void act(delta_time dt);

  const ui::drawable &overlay() const;

  /*!
    \brief Constructs an actor.

    This is the only way to create an actor that is to be used on stage. This
    member function accepts an arbitrary number of parameters that are forwarded
    to the actor's constructor.

    \tparam T The class of the actor. Must be the type of a concrete subclass of
    actor.
    \return A reference to the newly created actor. This reference is valid
    until the actor is removed from play (via \ref actor::remove or \ref
    stage::remove_actor) or the stage lifetime ends.
   */
  template <typename T, typename... Args>
  T &build_actor(Args &&... constructor_arguments) {
    static_assert(std::is_convertible<T *, actor *>::value,
                  "This function can only construct"
                  " concrete subclasses of actor");
    static_assert(std::is_constructible<T, Args...>::value,
                  "The requested type cannot be constructed from"
                  "the arguments provided.");
    actors_.push_back(
        std::make_unique<T>(std::forward<Args>(constructor_arguments)...));
    return *(static_cast<T *>(actors_.back().get()));
  }

  /*!
    \brief Removes an actor from play.

    This function removes the actor from the list of actors on stage. As the
    stage owns the actor, the actor will be removed from memory and the actor's
    destructor will be called. Any reference or pointer to that actor is now
    invalid.

    This function invalidates iterators and must not be called from any \a act
    method. If the method is called from an act function, an exception is
    thrown.

    \param a Actor to remove from play.
   */
  void remove_actor(actor const &a);

  iterator begin();
  iterator end();
  const_iterator cbegin() const;
  const_iterator cend() const;
  const_iterator begin() const;
  const_iterator end() const;

private:
  void check_interactions();
  math::size size_;

  friend class stage_overlay;
  std::unique_ptr<stage_overlay> overlay_;
  std::optional<quad_tree<4>> m_quad_tree_;

  container actors_{};
  bool acting_{};
};
} // namespace play
} // namespace kmint
#endif
