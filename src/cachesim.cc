// Copyright 2021 Juan Yaguaro
#include <cachesim/cache.h>
#include <cachesim/limits.h>
#include <cachesim/prefix.h>
#include <cachesim/version.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
static void one_argument(const std::string& arg);
static void many_arguments(const std::vector<std::string>& args);
static void get_option(const std::string& arg, std::string* config,
                       std::string* data, std::string* out, bool* hex);
static void simulate_allocation(const std::string& config_filename,
                                const std::string& data_filename,
                                const std::string& output_filename,
                                const bool& hex_output);
static std::unique_ptr<cachesim::cache> create_simulator(std::ifstream& is,
                                                         std::ostream& os,
                                                         const bool& hex);
static void allocate_data(std::ifstream& is,
                          std::unique_ptr<cachesim::cache>& caches);
static void print_header(std::ostream& os);
static void print_footer(std::ostream& os,
                         const std::unique_ptr<cachesim::cache>& caches);

// Main function
int main(int argc, char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  args.erase(args.begin());
  switch (args.size()) {
    case 1:
      one_argument(args[0]);
      break;
    case 2:
    case 3:
    case 4:
      many_arguments(args);
      break;
    default:
      std::cout << cachesim::cachesim_default;
      break;
  }

  return 0;
}

// Evaluates an argument looking for the prefix of version or help.
// It will output the help, version or default message depending on the argument
// prefix.
static void one_argument(const std::string& arg) {
  auto output_message = cachesim::is_version_prefix(arg)
                            ? cachesim::cachesim_version
                            : cachesim::is_help_prefix(arg)
                                  ? cachesim::cachesim_help
                                  : cachesim::error::invalid_argument;

  std::cout << output_message;
}

// Evaluates a vector of arguments to get the options inside them.
// It aslo generates the random number files depending if the given arguments
// were valid. Else, it will output the default message to std::cout.
static void many_arguments(const std::vector<std::string>& args) {
  auto invalid_argument_read = false;
  auto hex_output = false;
  std::string config_filename;
  std::string data_filename;
  std::string output_filename;

  for (const auto& arg : args) {
    try {
      get_option(arg, &config_filename, &data_filename, &output_filename,
                 &hex_output);
    } catch (const std::exception& e) {
      invalid_argument_read = true;
      config_filename.clear();
      break;
    }
  }

  if (!config_filename.empty() && !data_filename.empty()) {
    simulate_allocation(config_filename, data_filename, output_filename,
                        hex_output);
  } else {
    std::cout << (invalid_argument_read
                      ? cachesim::error::invalid_argument
                      : cachesim::error::no_input_files_detected);
  }
}

// Overwrites the pointer of the selected prefix.
// In case no prefix was found, it won't do anything (No option was found).
static void get_option(const std::string& arg, std::string* config,
                       std::string* data, std::string* out, bool* hex) {
  if (arg.size() > 4) {
    if (arg.rfind(cachesim::config_prefix, 0) == 0) {
      *config = arg.substr(3);
    } else if (arg.rfind(cachesim::data_prefix, 0) == 0) {
      *data = arg.substr(3);
    } else if (arg.rfind(cachesim::out_prefix, 0) == 0) {
      *out = arg.substr(3);
    } else {
      throw std::invalid_argument(cachesim::error::invalid_argument);
    }
  } else if (arg == cachesim::hex_prefix) {
    *hex = true;
  } else {
    throw std::invalid_argument(cachesim::error::invalid_argument);
  }
}

// Simulates the allocation of the addresses obtained in the  data file,
// configuring the cache depending on the parameters extracted from config file.
// It will redirect program output to the std::ostream specified.
static void simulate_allocation(const std::string& config_filename,
                                const std::string& data_filename,
                                const std::string& output_filename,
                                const bool& hex_output) {
  std::ifstream config_is(config_filename);
  std::ifstream data_is(data_filename);
  std::ofstream ofs(output_filename, std::ios::out);
  std::ostream& os = output_filename.empty() ? std::cout : ofs;

  if (config_is.is_open()) {
    std::unique_ptr<cachesim::cache> cache_simulator(
        create_simulator(config_is, os, hex_output));
    if (data_is.is_open()) {
      if (cache_simulator) {
        print_header(os);
        allocate_data(data_is, cache_simulator);
        print_footer(os, cache_simulator);
      } else {
        std::cout << cachesim::error::invalid_cache_size;
      }
    } else {
      std::cout << cachesim::error::failed_to_open << data_filename << '\n';
    }
  } else {
    std::cout << cachesim::error::failed_to_open << config_filename << '\n';
  }
}

// Returns a cachesim::cache instance depending on the config input file.
// It will check for the data beforehand, making sure that no invalid data was
// read.
static std::unique_ptr<cachesim::cache> create_simulator(std::ifstream& is,
                                                         std::ostream& os,
                                                         const bool& hex) {
  int size = 0;
  int type = 0;
  int line_size = 0;
  int policy = 0;
  std::unique_ptr<cachesim::cache> new_cache = nullptr;

  if (is >> size >> type >> line_size >> policy) {
    switch (type) {
      case 0:
        try {
          new_cache = std::make_unique<cachesim::direct_cache>(size, line_size,
                                                               policy, os, hex);
        } catch (const std::exception& e) {
          new_cache = nullptr;
        }
        break;
      case 1:
        try {
          new_cache = std::make_unique<cachesim::set_associative_cache>(
              size, line_size, policy, os, hex);
        } catch (const std::exception& e) {
          new_cache = nullptr;
        }
        break;
      default:
        std::cout << cachesim::error::invalid_cache_type;
        break;
    }
  } else {
    std::cout << cachesim::error::invalid_config_input;
  }

  return new_cache;
}

// Allocates the data read from the data file into the cache simulator.
static void allocate_data(std::ifstream& is,
                          std::unique_ptr<cachesim::cache>& caches) {
  int dir = 0;
  while (is >> dir) {
    caches->allocate(dir);
  }
}

// Outputs header content to the given std::ostream.
static void print_header(std::ostream& os) {
  os << std::setfill('-') << std::setw(106) << '\n';
  os.width(25);
  os << std::setfill(' ') << "Address to allocate";
  os.width(20);
  os << "Hit(1)/Miss(0)";
  os.width(10);
  os << "Set ID";
  os.width(25);
  os << "Old cache line content";
  os.width(25);
  os << "New cache line content" << '\n';
  os << std::setfill('-') << std::setw(106) << '\n';
  os << std::setfill(' ');
}

// Outputs footer content to the given std::ostream.
static void print_footer(std::ostream& os,
                         const std::unique_ptr<cachesim::cache>& caches) {
  double total{static_cast<double>(caches->hit_count()) +
               static_cast<double>(caches->miss_count())};
  double hit_freq{100 * static_cast<double>(caches->hit_count()) / total};
  double miss_freq{100 * static_cast<double>(caches->miss_count()) / total};

  os.width(25);
  os << "Total cache allocations: ";
  os.width(10);
  os << total << '\n';
  os.width(25);
  os << "Total cache hits: ";
  os.width(10);
  os << caches->hit_count() << '\n';
  os.width(25);
  os << "Total cache misses: ";
  os.width(10);
  os << caches->miss_count() << '\n';
  os.width(25);
  os << "Cache hit frequency: ";
  os.width(10);
  os << hit_freq << "%\n";
  os.width(25);
  os << "Cache miss frequency: ";
  os.width(10);
  os << miss_freq << "%\n";
}
