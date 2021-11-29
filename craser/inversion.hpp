#pragma once
#include <cmath>
#include <iostream>
#include <vector>
//
#include <craser/setup.hpp>
#include <craser/utility.hpp>

namespace craser {

inline auto inversion(setup s) {
  constexpr size_t n = 200;
  constexpr real precision = 1e-5;
  constexpr size_t max_iterations = 20;

  const real dz = s.crystal_length / (n - 1);
  const real dt = s.pump_duration / (n - 1);

  vector<real> initial_pump_rate(n * n);
  vector<real> beta(n * n, 0), pump_rate{}, old_beta{};
  old_beta = beta;
  vector<real> help_s(n * n);

  // initial pump rate
  for (size_t z = 0; z < n; ++z)
    initial_pump_rate[z] =
        s.initial_pump_rate * exp(-s.crystal_doping_concentration *
                                  s.pump_absorption_cross_section * dz * z);
  for (size_t t = 1; t < n; ++t)
    for (size_t z = 0; z < n; ++z)
      initial_pump_rate[t * n + z] = initial_pump_rate[(t - 1) * n + z];

  pump_rate = initial_pump_rate;

  real error = 0;
  size_t it = 0;

  do {
    swap(old_beta, beta);

    // Compute help_s.
    for (size_t z = 0; z < n; ++z) {
      real integral = 0;
      help_s[0 * n + z] = 1;
      for (size_t t = 1; t < n; ++t) {
        integral +=
            (pump_rate[(t - 1) * n + z] + pump_rate[t * n + z]) * dt / 2;
        help_s[t * n + z] =
            exp(integral * s.inv_beta_eq + s.inv_tau_f * dt * t);
      }
    }

    // Compute beta
    for (size_t z = 0; z < n; ++z) {
      real integral = 0;
      beta[0 * n + z] = 0;
      for (size_t t = 1; t < n; ++t) {
        integral += (help_s[(t - 1) * n + z] * pump_rate[(t - 1) * n + z] +
                     help_s[t * n + z] * pump_rate[t * n + z]) *
                    dt / 2;
        beta[t * n + z] = integral / help_s[t * n + z];
      }
    }

    // compute pump rate
    for (size_t t = 0; t < n; ++t) {
      real integral = 0;
      pump_rate[t * n + 0] = initial_pump_rate[t * n + 0];
      for (size_t z = 1; z < n; ++z) {
        integral += (beta[t * n + (z - 1)] + beta[t * n + z]) * dz / 2;
        pump_rate[t * n + z] =
            initial_pump_rate[t * n + z] *
            exp(s.crystal_doping_concentration *
                s.pump_absorption_cross_section * s.inv_beta_eq * integral);
      }
    }

    // condition
    error = 0;
    for (size_t t = 0; t < n; ++t) {
      for (size_t z = 0; z < n; ++z) {
        const auto a = old_beta[t * n + z] - beta[t * n + z];
        error += a * a;
      }
    }
    error = sqrt(error) / (n * n);
    ++it;
    cout << it << endl << error << endl;
  } while ((it < max_iterations) && (error > precision));

  vector<real> result(n);
  for (size_t z = 0; z < n; ++z) result[z] = beta[(n - 1) * n + z];
  return result;
}

}  // namespace craser
