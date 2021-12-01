#pragma once
#include <cmath>
#include <numbers>
#include <random>
//
#include <craser/amplification.hpp>
#include <craser/setup.hpp>
#include <craser/utility.hpp>

namespace craser {

inline auto travel_length(setup s, real x, real y, real z, real theta,
                          real phi) noexcept {
  const auto dir_x = sin(theta) * cos(phi);
  const auto dir_y = sin(theta) * sin(phi);
  const auto dir_z = cos(theta);

  const auto inv_dir_x = 1 / dir_x;
  const auto inv_dir_y = 1 / dir_y;
  const auto inv_dir_z = 1 / dir_z;

  const auto tx0 = -x * inv_dir_x;
  const auto ty0 = -y * inv_dir_y;
  const auto tz0 = -z * inv_dir_z;

  const auto tx1 = (s.crystal_width - x) * inv_dir_x;
  const auto ty1 = (s.crystal_width - y) * inv_dir_y;
  const auto tz1 = (s.crystal_length - z) * inv_dir_z;

  const auto tx_max = max(tx0, tx1);
  const auto ty_max = max(ty0, ty1);
  const auto tz_max = max(tz0, tz1);

  const auto u = min(tx_max, ty_max);
  const auto [t, is_z] =
      min(pair{u, false}, pair{tz_max, true},
          [](const auto& x, const auto& y) { return x.first <= y.first; });

  if (!is_z || (theta < s.crystal_total_reflection_angle) ||
      (theta > pi - s.crystal_total_reflection_angle))
    return t;
  return u;
}

inline auto ase(setup s, size_t samples) {
  mt19937 rng{random_device{}()};
  uniform_real_distribution<real> dist{0, 1};
  const auto random = [&] { return dist(rng); };

  real result = 0;
  for (size_t i = 0; i < samples; ++i) {
    const auto x = random() * s.crystal_width;
    const auto y = random() * s.crystal_width;
    const auto z = random() * s.crystal_length;

    const auto phi = random() * 2 * pi;
    const auto theta = acos(2 * random() - 1);

    const real amplification = 1.407602601;
    result += exp(amplification * travel_length(s, x, y, z, theta, phi));
  }
  result /= samples;
  return result;
}

}  // namespace craser
