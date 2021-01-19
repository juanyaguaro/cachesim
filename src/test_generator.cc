// Copyright 2020 Juan Yaguaro
#include <cachesim/limits.h>
#include <cachesim/prefix.h>
#include <cachesim/version.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

static int get_random(const int& lower_bound, const int& upper_bound);
static void generate_random_config(const std::string& filename);
static void generate_random_data(const std::string& filename);

int main(int argc, char* argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);

  if (args.size() == 1) {
    if (cachesim::is_version_prefix(argv[1])) {
      std::cout << cachesim::test_generator_version;
    } else if (cachesim::is_help_prefix(argv[1])) {
      std::cout << cachesim::test_generator_help;
    } else {
      std::cout << cachesim::test_generator_default;
    }
  } else if (args.size() == 2) {
    std::string config_filename;
    std::string data_filename;

    for (const auto& arg : args) {
      if (arg.rfind(cachesim::config_prefix, 0) == 0 && arg.size() > 4) {
        config_filename = arg.substr(3);
      } else if (arg.rfind(cachesim::data_prefix, 0) == 0 && arg.size() > 4) {
        data_filename = arg.substr(3);
      }
    }
    if (!config_filename.empty() && !data_filename.empty()) {
      generate_random_config(config_filename);
      generate_random_data(data_filename);
    } else {
      std::cout << cachesim::test_generator_default;
    }

  } else {
    std::cout << cachesim::test_generator_default;
  }
  return 0;
}

static int get_random(const int& lower_bound, const int& upper_bound) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(lower_bound, upper_bound);

  return distr(gen);
}

static void generate_random_config(const std::string& filename) {
  auto pow_cache_size =
      get_random(cachesim::limits::pow_min, cachesim::limits::pow_max);
  auto pow_line_size = get_random(cachesim::limits::pow_min, pow_cache_size);
  std::ofstream os(filename, std::ios::out);

  os << pow(2, pow_cache_size) << '\n'
     << get_random(0, 1) << '\n'
     << pow(2, pow_line_size) << '\n'
     << get_random(0, 1) << '\n';
}

static void generate_random_data(const std::string& filename) {
  auto n = get_random(cachesim::limits::it_min, cachesim::limits::it_max);
  std::ofstream os(filename, std::ios::out);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(cachesim::limits::num_min,
                                        cachesim::limits::num_max);

  for (auto i = 0; i < n; ++i) {
    os << distr(gen) << '\n';
  }
}
