// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_LIMITS_H_
#define CACHESIM_LIMITS_H_

#include <cstddef>
#include <limits>

namespace cachesim {
namespace limits {
constexpr const std::size_t it_min = 1;
constexpr const std::size_t it_max = 100;
constexpr const std::size_t num_min = 0;
constexpr const std::size_t num_max = 65535;
constexpr const std::size_t pow_min = 0;
constexpr const std::size_t pow_max = 16;
}  // namespace limits
}  // namespace cachesim

#endif  // CACHESIM_LIMITS_H_
