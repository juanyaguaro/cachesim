// Copyright 2021 Juan Yaguaro
#ifndef CACHESIM_LIMITS_H_
#define CACHESIM_LIMITS_H_

#include <cstddef>

namespace cachesim {
namespace limits {
// Number of iterations limits.
constexpr const std::size_t it_min = 1;
constexpr const std::size_t it_max = 100;

// Numeric values limits. Represents a max of 2^16 (emulating 16-bit
// architecture).
constexpr const std::size_t num_min = 0;
constexpr const std::size_t num_max = 65535;

// Power n values limits (2^n).
constexpr const std::size_t pow_min = 0;
constexpr const std::size_t pow_max = 16;
}  // namespace limits
}  // namespace cachesim

#endif  // CACHESIM_LIMITS_H_
