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
  auto [pulse, energy] = craser::pulse_amplification(setup, inversion, 6);

  // print("energy = {}\n", energy);

  gnuplot::temporary_file file{};
  for (auto x : pulse) file << x << '\n';
  file.flush();

  gnuplot::pipe plot{};
  plot << format("plot '{}' w l\n", file.path().string());
}
