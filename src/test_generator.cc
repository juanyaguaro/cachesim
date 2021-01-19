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

static void one_argument(const std::string& arg);
static void many_arguments(const std::vector<std::string>& args);
static void get_option(const std::string& arg, std::string* config,
                       std::string* data);
static int get_random(const int& lower_bound, const int& upper_bound);
static void generate_random_config(const std::string& filename);
static void generate_random_data(const std::string& filename);

int main(int argc, char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  args.erase(args.begin());
  switch (args.size()) {
    case 1:
      one_argument(args[0]);
      break;
    case 2:
      many_arguments(args);
      break;
    default:
      std::cout << cachesim::test_generator_default;
      break;
  }

  return 0;
}

static void one_argument(const std::string& arg) {
  auto output_message = cachesim::is_version_prefix(arg)
                            ? cachesim::test_generator_version
                            : cachesim::is_help_prefix(arg)
                                  ? cachesim::test_generator_help
                                  : cachesim::test_generator_default;

  std::cout << output_message;
}

static void many_arguments(const std::vector<std::string>& args) {
  std::string config_filename;
  std::string data_filename;

  for (const auto& arg : args) {
    get_option(arg, &config_filename, &data_filename);
  }

  if (!config_filename.empty() && !data_filename.empty()) {
    generate_random_config(config_filename);
    generate_random_data(data_filename);
  } else {
    std::cout << cachesim::test_generator_default;
  }
}

void get_option(const std::string& arg, std::string* config,
                std::string* data) {
  if (arg.size() > 4) {
    if (arg.rfind(cachesim::config_prefix, 0) == 0) {
      *config = arg.substr(3);
    } else if (arg.rfind(cachesim::data_prefix, 0) == 0) {
      *data = arg.substr(3);
    }
  }
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
