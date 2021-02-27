#include "kmint/play/actor.hpp"

namespace kmint::play {

void actor::register_collision(actor &p) { collision_set_.emplace_back(&p); }

void actor::register_perceived(actor &p) { perceived_set_.emplace_back(&p); }

add_collision_iterator &add_collision_iterator::operator=(actor &a) {
  if (owner_ != &a)
    owner_->collision_set_.push_back(&a);
  return *this;
}
add_collision_iterator &add_collision_iterator::operator=(actor *a) {
  if (owner_ != a)
    owner_->collision_set_.push_back(a);
  return *this;
}

add_perceived_iterator &add_perceived_iterator::operator=(actor &a) {
  if (owner_ != &a)
    owner_->perceived_set_.push_back(&a);
  return *this;
}
add_perceived_iterator &add_perceived_iterator::operator=(actor *a) {
  if (owner_ != a)
    owner_->perceived_set_.push_back(a);
  return *this;
}

} // namespace kmint::play
