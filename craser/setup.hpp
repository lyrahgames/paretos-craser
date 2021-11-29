#pragma once
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
  void compute() {
    pump_intensity = pump_energy / (pump_duration * crystal_area);
    initial_pump_rate = pump_intensity * pump_absorption_cross_section *
                        pump_wavelength / planck / light_speed;
    beta_eq = pump_absorption_cross_section /
              (pump_absorption_cross_section + pump_emission_cross_section);
    inv_beta_eq = 1 / beta_eq;
    inv_tau_f = 1 / tau_f;
  }

  real planck{6.626e-34};                        //
  real light_speed{3e8};                         // m*s^-1
  real pump_energy{600.0};                       // J
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

  real pump_intensity{};
  real initial_pump_rate{};
  real beta_eq{};
  real inv_beta_eq{};
  real inv_tau_f{};
};

}  // namespace craser
