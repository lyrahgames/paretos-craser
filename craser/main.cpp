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
  setup.compute();
  print("intensity = {}\n", setup.pump_intensity);
  const auto inversion = craser::inversion(setup);

  gnuplot::temporary_file file{};
  for (auto x : inversion) file << x << '\n';
  file.flush();

  gnuplot::pipe plot{};
  plot << format("plot '{}' w l\n", file.path().string());
}
