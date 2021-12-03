#include <iomanip>
//
#include <fmt/format.h>
//
#include <lyrahgames/gnuplot/gnuplot.hpp>
//
#include <craser/craser.hpp>

using namespace std;
using namespace fmt;
using namespace lyrahgames;

int main(int argc, char* argv[]) {
  craser::setup setup{};
  // auto inversion = craser::inversion(setup);
  // auto [pulse, energy] = craser::pulse_amplification(setup, inversion, 10);

  setup.crystal_length = 1.6358204297888304;
  setup.crystal_width = 1.6358204297888304;
  const auto ase = craser::ase(setup, 1'000'000);

  // print("ase = {}\n", ase);

  gnuplot::temporary_file file{};
  // for (int i = 0; auto x : inversion) {
  //   file << i << ' ' << setprecision(10) << x << '\n';
  //   ++i;
  // }
  // const float volume = 4.33;
  // const int n = 100;
  // for (int i = 0; i < n; ++i) {
  //   setup.crystal_length = (10.0f - 0.5f) / (n - 1) * i + 0.5f;
  //   setup.crystal_width = sqrt(volume / setup.crystal_length);
  //   file << setup.crystal_length / setup.crystal_width << ' '
  //        << craser::ase(setup, 1'000'000) << '\n';
  // }

  const size_t n = 500;
  const float xmin = 1.0e-6;
  const float xmax = 4.0e-6;
  for (size_t i = 0; i < n; ++i) {
    const float x = (xmax - xmin) / (n - 1) * i + xmin;
    const float y = craser::cr_zn_se::emission_cross_section(x);
    const float z = craser::cr_zn_se::absorption_cross_section(x);
    const float a = craser::cr_zn_s::absorption_cross_section(x);
    const float b = craser::cr_zn_s::emission_cross_section(x);
    file << x << ' ' << y << ' ' << z << ' ' << a << ' ' << b << '\n';
  }

  file.flush();

  gnuplot::pipe plot{};
  plot << format("plot '{}' u 1:2 w l\n", file.path().string())
       << format("replot '' u 1:3 w l\n")  //
       << format("replot '' u 1:4 w l\n")  //
       << format("replot '' u 1:5 w l\n")  //
      ;
}
