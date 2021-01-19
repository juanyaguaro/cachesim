// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_VERSION_H_
#define CACHESIM_VERSION_H_ "0.1.0"

namespace cachesim {

constexpr const char* cachesim_version =
    "cachesim " CACHESIM_VERSION_H_ "\nCopyright 2020 Juan Yaguaro.\n";

constexpr const char* test_generator_version =
    "test_generator " CACHESIM_VERSION_H_ "\nCopyright 2020 Juan Yaguaro.\n";

constexpr const char* test_generator_help =
    "Usage: test_generator -c=[VALUE] -d=[OPTION]\n"
    "\t-c=[VALUE]\t\tfilename for config file.\n"
    "\t-d=[VALUE]\t\tfilename for data file.\n"
    "\t-h, --help\t\tdisplay all available commands.\n"
    "\t-v, --version\t\tdisplay version of test_generator.\n"
    "Full documentation at: <https://github.com/juanyaguaro/cachesim>.\n"
    "Or available locally in <docs> folder.\n";

constexpr const char* cachesim_help =
    "Usage: cachesim -c=[VALUE] -d=[OPTION] -o=[OPTION]\n"
    "\t-c=[VALUE]\t\tfilename for config file.\n"
    "\t-d=[VALUE]\t\tfilename for data file.\n"
    "\t-d=[VALUE]\t\tfilename for output file (default value is stdout).\n"
    "\t-h, --help\t\tdisplay all available commands.\n"
    "\t-v, --version\t\tdisplay version of test_generator.\n"
    "Full documentation at: <https://github.com/juanyaguaro/cachesim>.\n"
    "Or available locally in <docs> folder.\n";

constexpr const char* test_generator_default =
    "test_generator - generates input files for cachesim.\n"
    "Try [test_generator -h] or [test_generator --help] for help.\n";

constexpr const char* cachesim_default =
    "cachesim - simulates cache behavior with given configuration and values "
    "to allocate.\n"
    "Try [cachesim -h] or [cachesim --help] for help.\n";

}  // namespace cachesim

#endif  // CACHESIM_VERSION_H_
