#pragma once
#include <cmath>
#include <numbers>
#include <span>
//
#include <lyrahgames/xstd/builtin_types.hpp>

namespace craser {

using namespace std;
using namespace lyrahgames::xstd;

using real = float32;

constexpr auto pi = numbers::pi_v<real>;
constexpr auto e = numbers::e_v<real>;

constexpr auto c = real(299792458);
constexpr auto h = real(6.6260688e-34);

constexpr auto linear_interpolation(const auto& data, auto x) noexcept {
  size_t left = 0;
  size_t right = size(data) - 1;

  const auto [a, p] = data[left];
  if (x <= a) return p;

  const auto [b, q] = data[right];
  if (x >= b) return q;

  while (right - left > 1) {
    size_t middle = (right + left) / 2;
    const auto [t, _] = data[middle];
    if (x <= t)
      right = middle;
    else
      left = middle;
  }

  const auto [x0, y0] = data[left];
  const auto [x1, y1] = data[right];
  return ((y1 - y0) / (x1 - x0) * (x - x0) + y0);
}

}  // namespace craser
