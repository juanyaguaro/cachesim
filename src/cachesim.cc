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
                       std::string* data, std::string* out);
static void simulate_allocation(const std::string& config_filename,
                                const std::string& data_filename,
                                const std::string& output_filename);
static std::unique_ptr<cachesim::cache> create_simulator(std::ifstream& is,
                                                         std::ostream& os);
static void allocate_data(std::ifstream& is,
                          std::unique_ptr<cachesim::cache>& caches);
static void print_header(std::ostream& os, const char* dir,
                         const char* hit_miss, const char* id,
                         const char* old_dir, const char* new_dir);
static void print_footer(std::ostream& os, const char* alloc_total,
                         const char* hit_total, const char* miss_total,
                         const char* hit_frequency, const char* miss_frequency,
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
                                  : cachesim::cachesim_default;

  std::cout << output_message;
}

// Evaluates a vector of arguments to get the options inside them.
// It aslo generates the random number files depending if the given arguments
// were valid. Else, it will output the default message to std::cout.
static void many_arguments(const std::vector<std::string>& args) {
  std::string config_filename;
  std::string data_filename;
  std::string output_filename;

  for (const auto& arg : args) {
    get_option(arg, &config_filename, &data_filename, &output_filename);
  }

  if (!config_filename.empty() && !data_filename.empty()) {
    simulate_allocation(config_filename, data_filename, output_filename);
  } else {
    std::cout << cachesim::cachesim_default;
  }
}

// Overwrites the pointer of the selected prefix.
// In case no prefix was found, it won't do anything (No option was found).
static void get_option(const std::string& arg, std::string* config,
                       std::string* data, std::string* out) {
  if (arg.size() > 4) {
    if (arg.rfind(cachesim::config_prefix, 0) == 0) {
      *config = arg.substr(3);
    } else if (arg.rfind(cachesim::data_prefix, 0) == 0) {
      *data = arg.substr(3);
    } else if (arg.rfind(cachesim::out_prefix, 0) == 0) {
      *out = arg.substr(3);
    }
  }
}

// Simulates the allocation of the directions obtained in the  data file,
// configuring the cache depending on the parameters extracted from config file.
// It will redirect program output to the std::ostream specified.
static void simulate_allocation(const std::string& config_filename,
                                const std::string& data_filename,
                                const std::string& output_filename) {
  std::ifstream config_is(config_filename);
  std::ifstream data_is(data_filename);
  std::ofstream ofs(output_filename, std::ios::out);
  std::ostream& os = output_filename.empty() ? std::cout : ofs;

  if (config_is.is_open()) {
    std::unique_ptr<cachesim::cache> cache_simulator(
        create_simulator(config_is, os));
    if (data_is.is_open()) {
      print_header(os, "Direction to allocate", "Hit(1)/Miss(0)", "Set ID",
                   "Old cache line content", "New cache line content");
      allocate_data(data_is, cache_simulator);
      print_footer(os, "Total cache allocations: ", "Total cache hits: ",
                   "Total cache misses: ", "Cache hit frequency: ",
                   "Cache miss frequency: ", cache_simulator);
    } else {
      std::cout << "Failed to open " << data_filename << '\n';
    }
  } else {
    std::cout << "Failed to open " << config_filename << '\n';
  }
}

// Returns a cachesim::cache instance depending on the config input file.
// It will check for the data beforehand, making sure that no invalid data was
// read.
static std::unique_ptr<cachesim::cache> create_simulator(std::ifstream& is,
                                                         std::ostream& os) {
  int size = 0;
  int type = 0;
  int line_size = 0;
  int policy = 0;
  std::unique_ptr<cachesim::cache> new_cache = nullptr;

  if (is >> size >> type >> line_size >> policy) {
    switch (type) {
      case 0:
        new_cache = std::make_unique<cachesim::direct_cache>(size, line_size,
                                                             policy, os);
        break;
      case 1:
        new_cache = std::make_unique<cachesim::set_associative_cache>(
            size, line_size, policy, os);
        break;
      default:
        std::cout << "Invalid cache type read in config file.\n";
        break;
    }
  } else {
    std::cout << "Invalid input read in config file.\n";
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
static void print_header(std::ostream& os, const char* dir,
                         const char* hit_miss, const char* id,
                         const char* old_dir, const char* new_dir) {
  os.width(25);
  os << dir;
  os.width(20);
  os << hit_miss;
  os.width(10);
  os << id;
  os.width(25);
  os << old_dir;
  os.width(25);
  os << new_dir << '\n';
}

// Outputs footer content to the given std::ostream.
static void print_footer(std::ostream& os, const char* alloc_total,
                         const char* hit_total, const char* miss_total,
                         const char* hit_frequency, const char* miss_frequency,
                         const std::unique_ptr<cachesim::cache>& caches) {
  double total{static_cast<double>(caches->hit_count()) +
               static_cast<double>(caches->miss_count())};
  double hit_freq{100 * static_cast<double>(caches->hit_count()) / total};
  double miss_freq{100 * static_cast<double>(caches->miss_count()) / total};

  os << alloc_total << total << '\n'
     << hit_total << caches->hit_count() << '\n'
     << miss_total << caches->miss_count() << '\n'
     << hit_frequency << hit_freq << "%\n"
     << miss_frequency << miss_freq << "%\n";
}
