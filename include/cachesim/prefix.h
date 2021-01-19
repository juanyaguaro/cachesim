// Copyright 2021 Juan Yaguaro
#ifndef CACHESIM_PREFIX_H_
#define CACHESIM_PREFIX_H_

#include <string_view>

namespace cachesim {

// Version prefixes
constexpr const std::string_view version_prefix_s = "-v";
constexpr const std::string_view version_prefix_l = "--version";
// Help prefixes
constexpr const std::string_view help_prefix_s = "-h";
constexpr const std::string_view help_prefix_l = "--help";

// Program options prefixes
constexpr const std::string_view config_prefix = "-c=";
constexpr const std::string_view data_prefix = "-d=";
constexpr const std::string_view out_prefix = "-o=";

// Returns whether the given value is a version prefix.
bool is_version_prefix(std::string_view s) {
  return s == version_prefix_s || s == version_prefix_l;
}

// Returns whether the given value is a help prefix.
bool is_help_prefix(std::string_view s) {
  return s == help_prefix_s || s == help_prefix_l;
}

}  // namespace cachesim

#endif  // CACHESIM_PREFIX_H_
