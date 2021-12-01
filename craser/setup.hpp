#pragma once
#include <cmath>
#include <craser/utility.hpp>

namespace craser {

struct crystal {
  constexpr auto length() const noexcept { return l; }
  constexpr auto area() const noexcept { return A; }
  constexpr auto doping_contentration() const noexcept { return N_D; }

  real l;
  real A;
  real N_D;
};

struct setup {
  setup() { compute(); }

  void compute() {
    pump_intensity = pump_energy / (pump_duration * crystal_area);
    initial_pump_rate = pump_intensity * pump_absorption_cross_section *
                        pump_wavelength / h / c;
    beta_eq = pump_absorption_cross_section /
              (pump_absorption_cross_section + pump_emission_cross_section);
    inv_beta_eq = 1 / beta_eq;
    inv_tau_f = 1 / tau_f;
    seed_fluence = seed_energy / crystal_area;
    laser_beta_eq =
        laser_absorption_cross_section /
        (laser_absorption_cross_section + laser_emission_cross_section);

    crystal_width = sqrt(crystal_area);
    crystal_total_reflection_angle = asin(1 / crystal_refractive_index);
  }

  constexpr auto initial_pulse(real t) {
    return (abs(t) < seed_duration / 2)
               ? (laser_wavelength / (c * c * h) * seed_fluence / seed_duration)
               : 0;
  }

  real pump_energy{60.0};                        // J
  real pump_wavelength{940e-9};                  // m
  real pump_duration{2e-3};                      // s
  real pump_absorption_cross_section{0.8e-24};   // m^2
  real pump_emission_cross_section{0.16e-24};    // m^2
  real laser_wavelength{1030e-9};                // m
  real laser_absorption_cross_section{1.1e-25};  // m^2
  real laser_emission_cross_section{2.3e-24};    // m^2
  real crystal_doping_concentration{6e26};       // m^-3
  real crystal_length{1e-2};                     // m
  real crystal_area{1e-4};                       // m^2
  real tau_f{9.5e-4};                            // s
  real seed_duration{5e-9};                      // s
  real seed_energy{1e-2};                        // J
  real losses{0.02};
  real crystal_refractive_index = 2.4;  //

  real crystal_total_reflection_angle{};
  real seed_fluence{};  // J/m^2
  real pump_intensity{};
  real initial_pump_rate{};
  real beta_eq{};
  real inv_beta_eq{};
  real laser_beta_eq{};
  real inv_tau_f{};
  real crystal_width{};
};

}  // namespace craser
