//
// cpp03/can_require_not_applicable_free.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "asio/require.hpp"
#include <cassert>

template <int>
struct prop
{
  static const bool is_requirable = true;
};

template <int>
struct object
{
  template <int N>
  friend object<N> require(const object&, prop<N>)
  {
    return object<N>();
  }
};

namespace asio {
namespace traits {

template<int N, int M>
struct require_free<object<N>, prop<M> >
{
  static const bool is_valid = true;
  static const bool is_noexcept = true;
  typedef object<M> result_type;
};

} // namespace traits
} // namespace asio

int main()
{
  assert((!asio::can_require<object<1>, prop<2> >::value));
  assert((!asio::can_require<object<1>, prop<2>, prop<3> >::value));
  assert((!asio::can_require<object<1>, prop<2>, prop<3>, prop<4> >::value));
  assert((!asio::can_require<const object<1>, prop<2> >::value));
  assert((!asio::can_require<const object<1>, prop<2>, prop<3> >::value));
  assert((!asio::can_require<const object<1>, prop<2>, prop<3>, prop<4> >::value));
}
