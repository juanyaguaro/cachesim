// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_VERSION_H_
#define CACHESIM_VERSION_H_ "0.1.0"

#include <string>

namespace cachesim {

std::string cachesim_version() {
  return std::string("cachesim " CACHESIM_VERSION_H_
                     "\nCopyright 2020 Juan Yaguaro.\n");
}

std::string test_generator_version() {
  return std::string("test_generator " CACHESIM_VERSION_H_
                     "\nCopyright 2020 Juan Yaguaro.\n");
}

std::string test_generator_help() {
  return std::string(
      "Usage: test_generator -c=[VALUE] -d=[OPTION]\n"
      "\t-c=[VALUE]\t\tfilename for config file.\n"
      "\t-d=[VALUE]\t\tfilename for data file.\n"
      "\t-h, --help\t\tdisplay all available commands.\n"
      "\t-v, --version\t\tdisplay version of test_generator.\n"
      "Full documentation at: <https://github.com/juanyaguaro/cachesim>.\n"
      "Or available locally in <docs> folder.\n");
}

std::string test_generator_default() {
  return std::string(
      "test_generator - generates input files for cachesim.\n"
      "Try [test_generator -h] or [test_generator --help] for help.\n");
}

}  // namespace cachesim

#endif  // CACHESIM_VERSION_H_
