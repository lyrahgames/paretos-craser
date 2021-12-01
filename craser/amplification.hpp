#pragma once
#include <vector>
//
#include <craser/inversion.hpp>
#include <craser/setup.hpp>
#include <craser/utility.hpp>

namespace craser {

inline auto pulse_amplification(setup s, vector<real>& inversion,
                                size_t passes) {
  constexpr size_t n = 200;
  constexpr real precision = 1e-5;

  const real dz = s.crystal_length / (n - 1);
  const real dt = 2 * s.seed_duration / (n - 1);

  vector<real> initial_pulse(n);
  for (size_t t = 0; t < n; ++t)
    initial_pulse[t] = s.initial_pulse(dt * t - s.seed_duration);

  auto pulse = initial_pulse;
  real pulse_energy;

  vector<real> delta(n);
  for (size_t z = 0; z < n; ++z)
    delta[z] = s.crystal_doping_concentration *
               (inversion[z] - s.laser_beta_eq) / (1 - s.laser_beta_eq);

  for (size_t it = 0; it < passes; ++it) {
    //
    vector<real> u(n);
    {
      real integral = 0;
      u[0] = 1;
      for (size_t z = 1; z < n; ++z) {
        integral += (delta[z - 1] + delta[z]) * dz / 2;
        u[z] = exp(-s.laser_emission_cross_section * integral);
        // integral += (inversion[z - 1] + inversion[z]) * dz / 2;
        // u[z] = exp(-s.crystal_doping_concentration *
        //            s.laser_emission_cross_section / (1 - s.laser_beta_eq) *
        //            (integral - dz * z * s.laser_beta_eq));
      }
    }
    const auto u_area = u.back();

    //
    vector<real> v(n);
    {
      real integral = 0;
      v[0] = 1;
      for (size_t t = 1; t < n; ++t) {
        integral += (pulse[t - 1] + pulse[t]) * dt / 2;
        v[t] = exp(-c * s.laser_emission_cross_section / (1 - s.laser_beta_eq) *
                   integral);
      }
    }
    const auto v_area = v.back();

    for (size_t t = 0; t < n; ++t)
      pulse[t] *= (1 - s.losses) / (1 - (1 - u_area) * v[t]);
    for (size_t z = 0; z < n; ++z)  //
      delta[z] *= (1 - s.losses) * u[z] / (u[z] + 1 / v_area - 1);
    //   inversion[z] = (1 - s.losses) * (inversion[z] - s.laser_beta_eq) * u[z]
    //   /
    //                      (u[z] + 1 / v_area - 1) +
    //                  s.laser_beta_eq;

    for (size_t z = 0; z < (n + 1) / 2; ++z) swap(delta[z], delta[n - 1 - z]);
    // swap(inversion[z], inversion[n - 1 - z]);

    pulse_energy = (pulse[0] + pulse[n - 1]) / 2;
    for (size_t t = 1; t < n - 1; ++t) pulse_energy += pulse[t];
    pulse_energy *= dt * c * c * h / s.laser_wavelength;

    cout << "energy = " << pulse_energy << endl;
  }

  for (size_t z = 0; z < n; ++z)
    inversion[z] =
        delta[z] * (1 - s.laser_beta_eq) / s.crystal_doping_concentration +
        s.laser_beta_eq;

  return pair{pulse, pulse_energy};
}

}  // namespace craser
