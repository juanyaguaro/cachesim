# cachesim

A cache simulator made with modern C++.

The purpose of this project is to simulate the behavior of cache memory with different given configuration files.

It's made with C++17 features and tested in Linux (gcc) and Windows (MinGW).

The input files requested are configuration file and data file.

The configuration file has the following structure:
```
Cache size in bytes (Represented by a power of 2 integer).
Cache type (0 for direct mapped, 1 for set-associative mapped).
Cache line size in bytes (Represented by a power of 2 integer, can't be greater than the cache size).
Replace policy (0 for LRU, 1 for MRU).
```

The data file has the following structure:
```
Address 0 (integer greater than 1).
Address 1.
.
.
Address n.
```
The address limit is set to be 2^16. This is to simulate the behavior of a 16-bit main memory.

The cache simulator will take the configuration file to modify the cache structure.
Then it will take the data file to allocate all addresses and output the result of every allocation to the given output stream.

At the end it will output the total number of allocations made, the total cache hits, the total cache misses the frequency for both cache hits and misses.

The output structure is the following:
```
Header
Address 0          Hit/Miss          Address Set ID          Old Cache Line Content          New Cache Line Content
Address 1 ""
.
.
.
Address n ""
Total Cache Allocations
Total Cache Hits
Total Cache Misses
Cache Hit Frequency
Cache Miss Frequency
```

## Installation of cachesim

g++ is required (at least gcc 8).

Compile with:

```bash
make
```

## Usage of cachesim

```bash
cachesim -c=config_filename -d=data_filename -o=output_filename -x
```

-c takes the cache configuration input filename.

-d takes the data input filename.

-o takes the output filename (if not present, default output will be std::cout.

-x will output the addresses in its hex value (if not present, default output will be decimal).

Options -c and -d are required.

Options -o and -x are optional.

You can also get the version running:
```bash
cachesim -v
or
cachesim --version
```

And get help with:
```bash
cachesim -h
or
cachesim --help
```

Make sure that you've added cachesim to PATH.

## test_generator

Input files generator for cachesim made with modern C++.

It will generate random number for both configuration and data file.

In the configuration file it will generate the following file structure:

```
Random power of 2 (max is 2^16).
Random 0 or 1.
Random power of 2 (max is equal to first line number).
Random 0 or 1.
```

In the data file it will generate the following file structure:
```
Random number (between 0 and 2^16).
Random number (between 0 and 2^16).
.
.
.
Random number (between 0 and 2^16).
```

The distribution of these random numbers is set to be within a random sized vector of previously selected numbers.

Then it will generate the file with those selected numbers, favoring the address repetition to boost the cache hit frequency in cachesim output.

## Installation of test_generator

g++ is required (at least gcc 8).

Compile with:

```bash
make test_generator
```

## Usage of test_generator

```bash
test_generator -c=config_filename -d=data_filename
```

-c takes the cache configuration output filename.

-d takes the data output filename.

Options -c and -d are required.

You can also get the version running:
```bash
test_generator -v
or
test_generator --version
```

And get help with:
```bash
test_generator -h
or
test_generator --help
```

Make sure that you've added test_generator to PATH.

## Contributing
This project won't receive more updates, pull requests are welcome though. Please open an issue whenever you encounter with a bug.

## Version

0.2.1

## License
[MIT](https://github.com/juanyaguaro/cachesim/blob/main/LICENSE)
