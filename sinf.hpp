#ifndef VXL_SINF_HPP
# define VXL_SINF_HPP
# pragma once

#include <cmath>

#include "vector.hpp"

namespace vxl
{

namespace detail
{

namespace constantsf
{
  inline constexpr auto FOPI = 1.27323954473516f;

  inline constexpr auto DP1 = 0.78515625f;
  inline constexpr auto DP2 = 2.4187564849853515625e-4f;
  inline constexpr auto DP3 = 3.77489497744594108e-8f;
}

}

template <unsigned N>
inline constexpr vector<float, N> sin(vector<float, N> xx) noexcept
{
  using int_value_type = typename vector_traits<float, N>::int_value_type;
  using int_vector_type = typename vector_traits<float, N>::int_vector_type;

  auto& x(xx.ref());
  auto& xi((int_vector_type&)(x));

  auto sign(int_vector_type(x) & cvector<int_value_type, N>(1 << 31));

  xi &= cvector<int_value_type, N>(~(1 << 31));

  auto j(convert<int_value_type, N>(
    cvector<float, N>(detail::constantsf::FOPI) * x));

  j += j & cvector<int_value_type, N>(1);

  auto y(convert<float, N>(j));

  sign ^= (j & cvector<int_value_type, N>(4)) <<
    cvector<int_value_type, N>(29);

  x = ((x - y * cvector<float, N>(detail::constantsf::DP1)) -
    y * cvector<float, N>(detail::constantsf::DP2)) -
    y * cvector<float, N>(detail::constantsf::DP3);

  auto const z(x * x);

	auto const y1(((cvector<float, N>(2.443315711809948e-5f) * z -
    cvector<float, N>(1.388731625493765e-3f)) * z +
    cvector<float, N>(4.166664568298827e-2f)) * z * z -
    cvector<float, N>(.5f) * z + cvector<float, N>(1.f));

	auto const y2((((cvector<float, N>(-1.9515295891e-4f) * z +
    cvector<float, N>(8.3321608736e-3f)) * z -
    cvector<float, N>(1.6666654611e-1f)) * z * x) + x);

  auto r(select(y1, y2, int_vector_type(cvector<int_value_type, N>(2) == j)));
  (int_vector_type&)(r) ^= sign;

  return {r};
}

template <unsigned N>
inline constexpr vector<float, N> cos(vector<float, N> xx) noexcept
{
  using int_value_type = typename vector_traits<float, N>::int_value_type;
  using int_vector_type = typename vector_traits<float, N>::int_vector_type;

  auto& x(xx.ref());
  auto& xi((int_vector_type&)(x));

  xi &= cvector<int_value_type, N>(~(1 << 31));

  auto j(convert<int_value_type, N>(
    cvector<float, N>(detail::constantsf::FOPI) * x));

  j += j & cvector<int_value_type, N>(1);

  auto y(convert<float, N>(j));

  auto const sign((~(j - cvector<int_value_type, N>(2)) &
    cvector<int_value_type, N>(4)) << cvector<int_value_type, N>(29));

  x = ((x - y * cvector<float, N>(detail::constantsf::DP1)) -
    y * cvector<float, N>(detail::constantsf::DP2)) -
    y * cvector<float, N>(detail::constantsf::DP3);

  auto const z(x * x);

	auto const y1(((cvector<float, N>(2.443315711809948e-5f) * z -
    cvector<float, N>(1.388731625493765e-3f)) * z +
    cvector<float, N>(4.166664568298827e-2f)) * z * z -
    cvector<float, N>(.5f) * z + cvector<float, N>(1.f));

	auto const y2((((cvector<float, N>(-1.9515295891e-4f) * z +
    cvector<float, N>(8.3321608736e-3f)) * z -
    cvector<float, N>(1.6666654611e-1f)) * z * x) + x);

  auto r(select(y2, y1, int_vector_type(cvector<int_value_type, N>(2) == j)));
  (int_vector_type&)(r) ^= sign;

  return {r};
}

template <unsigned N>
inline constexpr std::pair<vector<float, N>, vector<float, N> >
//__attribute__ ((noinline))
sincos(vector<float, N> xx) noexcept
{
  using int_value_type = typename vector_traits<float, N>::int_value_type;
  using int_vector_type = typename vector_traits<float, N>::int_vector_type;

  auto& x(xx.ref());
  auto& xi((int_vector_type&)(x));

  auto sign_sin(xi & cvector<int_value_type, N>(1 << 31));

  xi &= cvector<int_value_type, N>(~(1 << 31));

  auto j(convert<int_value_type, N>(
    cvector<float, N>(detail::constantsf::FOPI) * x));

  j += j & cvector<int_value_type, N>(1);

  auto y(convert<float, N>(j));

  sign_sin ^= (j & cvector<int_value_type, N>(4)) <<
    cvector<int_value_type, N>(29);

  // shift bit at index 2 into 1, if it is set and bit at index 1 is not set
  auto const sign_cos((~(j - cvector<int_value_type, N>(2)) &
    cvector<int_value_type, N>(4)) << cvector<int_value_type, N>(29));

  x = ((x - y * cvector<float, N>(detail::constantsf::DP1)) -
    y * cvector<float, N>(detail::constantsf::DP2)) -
    y * cvector<float, N>(detail::constantsf::DP3);

  auto const z(x * x);

	auto const y1(((cvector<float, N>(2.443315711809948e-5f) * z -
    cvector<float, N>(1.388731625493765e-3f)) * z +
    cvector<float, N>(4.166664568298827e-2f)) * z * z -
    cvector<float, N>(.5f) * z + cvector<float, N>(1.f));

	auto const y2((((cvector<float, N>(-1.9515295891e-4f) * z +
    cvector<float, N>(8.3321608736e-3f)) * z -
    cvector<float, N>(1.6666654611e-1f)) * z * x) + x);

  int_vector_type const sel(cvector<int_value_type, N>(2) == j);

  auto p1(select(y1, y2, sel));
  (int_vector_type&)(p1) ^= sign_sin;

  auto p2(select(y2, y1, sel));
  (int_vector_type&)(p2) ^= sign_cos;

  return {{p1}, {p2}};
}

}

#endif // VXL_SINF_HPP
