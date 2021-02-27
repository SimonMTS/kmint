#ifndef KMINT_UTIL_FINALLY_HPP
#define KMINT_UTIL_FINALLY_HPP

#include <type_traits>
#include <utility>

namespace kmint {

template <typename F> class finally {
public:
  finally(F &&f) : f_{std::forward<F>(f)} {}
  ~finally() { f_(); }
  finally(finally const &) = delete;
  finally(finally &&) = delete;
  finally &operator=(finally const &) = delete;
  finally &operator=(finally &&) = delete;

private:
  F f_;
};

template <typename F> finally(F &&f)->finally<std::decay_t<F>>;

} // namespace kmint

#endif /* KMINT_UTIL_FINALLY_HPP */
