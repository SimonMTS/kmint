#ifndef KMINT_UTIL_OVERLOAD_HPP
#define KMINT_UTIL_OVERLOAD_HPP

namespace kmint {

template<typename... Ts> struct overload : Ts... { using Ts::operator()...; };
template<typename... Ts> overload(Ts...) -> overload<Ts...>;

}  // kmint



#endif /* KMINT_UTIL_OVERLOAD_HPP */
