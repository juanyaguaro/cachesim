// Copyright 2021 Juan Yaguaro
#ifndef CACHESIM_ERROR_H_
#define CACHESIM_ERROR_H_

namespace cachesim {
namespace error {
// Invalid argument output.
constexpr const char* invalid_argument = "Error: Invalid argument received.\n";

// No input files output.
constexpr const char* no_input_files_detected =
    "Error: No input files detected.\n";

// Failed to open file output.
constexpr const char* failed_to_open = "Error: Failed to open ";

// Invalid configuration file output.
constexpr const char* invalid_config_input =
    "Error: Invalid input read in config file.\n";

// Invalid cache type output.
constexpr const char* invalid_cache_type =
    "Error: Invalid cache type read in config file.\n";

// Invalid cache size output.
constexpr const char* invalid_cache_size = "Error: Invalid cache size.\n";
}  // namespace error
}  // namespace cachesim

#endif  // CACHESIM_ERROR_H_
