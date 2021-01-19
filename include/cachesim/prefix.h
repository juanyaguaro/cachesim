// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_PREFIX_H_
#define CACHESIM_PREFIX_H_

#include <string_view>

namespace cachesim {

constexpr const std::string_view version_prefix_s = "-v";
constexpr const std::string_view version_prefix_l = "--version";
constexpr const std::string_view help_prefix_s = "-h";
constexpr const std::string_view help_prefix_l = "--help";

constexpr const std::string_view config_prefix = "-c=";
constexpr const std::string_view data_prefix = "-d=";
constexpr const std::string_view out_prefix = "-o=";

bool is_version_prefix(std::string_view s) {
  return s == version_prefix_s || s == version_prefix_l;
}

bool is_help_prefix(std::string_view s) {
  return s == help_prefix_s || s == help_prefix_l;
}

}  // namespace cachesim

#endif  // CACHESIM_PREFIX_H_
