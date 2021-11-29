#include <fmt/format.h>
//
#include <lyrahgames/gnuplot/gnuplot.hpp>

using namespace std;
using namespace fmt;
using namespace lyrahgames;

int main(int argc, char* argv[]) {
  gnuplot::pipe plot{};
  plot << "plot sin(x)\n";
}
