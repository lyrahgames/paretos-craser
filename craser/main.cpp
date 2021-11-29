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
  auto inversion = craser::inversion(setup);
  auto [pulse, energy] = craser::pulse_amplification(setup, inversion, 10);

  // print("energy = {}\n", energy);

  gnuplot::temporary_file file{};
  for (int i = 0; auto x : inversion) {
    file << i << ' ' << setprecision(10) << x << '\n';
    ++i;
  }
  file.flush();

  gnuplot::pipe plot{};
  plot << format("plot '{}' u 1:2 w lp\n", file.path().string());
}
