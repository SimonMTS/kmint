#include "kmint/play/stage.hpp"
#include "kmint/graphics/color.hpp"
#include "kmint/play/actor.hpp"
#include "kmint/ui/drawable.hpp"
#include "kmint/ui/window.hpp"
#include "kmint/util/finally.hpp"
#include <algorithm>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <vector>

namespace kmint {
namespace play {

namespace {
template <typename ForwardIt>
quad_tree<4> build_quad_tree(math::size size, ForwardIt begin, ForwardIt end) {
  quad_tree<4> qt{math::rectangle{{0.f, 0.f}, size}};
  for (auto i = begin; i != end; ++i) {
    qt.add(*i);
  }
  return qt;
}
} // namespace

class stage_overlay : public ui::drawable {
public:
  stage_overlay(stage const &s) : s_{&s} {}
  void draw(ui::frame &f) const override {
    if (s_->m_quad_tree_) {
      s_->m_quad_tree_->visit_free([&f](auto area, auto begin, auto end) {
        f.draw_rectangle_outline(area, kmint::graphics::colors::green);
      });
    }
  }

private:
  stage const *s_{};
};

stage::stage(math::size size)
    : size_{size}, overlay_{std::make_unique<stage_overlay>(*this)} {}

stage::~stage() = default;

void stage::check_interactions() {
  m_quad_tree_ = build_quad_tree(size_, begin(), end());
  std::vector<actor *> candidates{}; // used to store candidates
  for (actor &a : *this) {
    if (!a.incorporeal()) {
      m_quad_tree_->find(a.collision_box(), std::back_inserter(candidates));
      std::copy_if(
          candidates.begin(), candidates.end(), a.collision_adder(),
          [](actor const *candidate) { return !candidate->incorporeal(); });
      candidates.clear();
    }
    if (a.perceptive()) {
      m_quad_tree_->find(a.perception_box(), std::back_inserter(candidates));
      std::copy_if(
          candidates.begin(), candidates.end(), a.perceived_adder(),
          [](actor const *candidate) { return candidate->perceivable(); });
      candidates.clear();
    }
  }
}

void stage::act(delta_time dt) {
  acting_ = true;
  finally f{[&] { acting_ = false; }};

  check_interactions();

  for (actor &a : *this) {
    a.act(dt);
  }

  auto it = actors_.begin();
  while (it != actors_.end()) {
    if (auto &aptr = *it; aptr->removed()) {
      it = actors_.erase(it);
    } else {
      aptr->empty_collisions();
      aptr->empty_perceived();
      ++it;
    }
  }
}

const ui::drawable &stage::overlay() const { return *overlay_; }

void stage::remove_actor(actor const &a) {
  if (acting_)
    throw std::logic_error{"Cannot call remove_actor while game logic is "
                           "executing. Use actor::remove instead"};
  actors_.erase(
      std::remove_if(actors_.begin(), actors_.end(),
                     [&a](auto const &ptr) { return &a == ptr.get(); }),
      actors_.end());
}
stage::iterator stage::begin() {
  return {actors_.begin(), util::deref_unique_ptr<actor>};
}
stage::iterator stage::end() {
  return {actors_.end(), util::deref_unique_ptr<actor>};
}
stage::const_iterator stage::cbegin() const {
  return {actors_.begin(), util::deref_unique_ptr_const<actor>};
}
stage::const_iterator stage::cend() const {
  return {actors_.end(), util::deref_unique_ptr_const<actor>};
}
stage::const_iterator stage::begin() const { return cbegin(); }
stage::const_iterator stage::end() const { return cend(); }

} // namespace play

} // namespace kmint
