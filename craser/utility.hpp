#pragma once
#include <cmath>
#include <numbers>
//
#include <lyrahgames/xstd/builtin_types.hpp>

namespace craser {

using namespace std;
using namespace lyrahgames::xstd;

using real = float32;

constexpr auto pi = numbers::pi_v<real>;
constexpr auto e = numbers::e_v<real>;

constexpr auto c = real(299792458);
constexpr auto h = real(6.6260688e-34);

}  // namespace craser
