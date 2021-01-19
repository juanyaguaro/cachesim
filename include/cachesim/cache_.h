// Copyright 2021 Juan Yaguaro
#ifndef CACHESIM_CACHE__H_
#define CACHESIM_CACHE__H_

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace cachesim {

// enum emplace_policy
// Defines the emplace algorithm to use.
enum emplace_policy { LRU, MRU };

// constant empty_space
// Defines an empty space in cache.
constexpr int empty_space = -1;

// class cache
// Abstract definition of a cache.
class cache {
 public:
  // ctor
  cache();
  explicit cache(const std::size_t& size, const std::size_t& line_size,
                 const int& policy, std::ostream& os);
  // dtor
  virtual ~cache() = default;
  // accessors
  std::size_t size() const noexcept;
  std::size_t line_size() const noexcept;
  std::size_t count() const noexcept;
  int hit_count() const noexcept;
  int miss_count() const noexcept;
  // mutators
  virtual void clear() = 0;
  virtual void resize(const std::size_t& size,
                      const std::size_t& line_size) = 0;
  virtual void emplace(const int& value) = 0;

 protected:
  virtual int get_id(const int& value) const noexcept = 0;
  bool is_pow2(const std::size_t& n) const noexcept;
  void print_line(const int& dir, const bool& hit_miss, const int& id,
                  const int& old_dir) const noexcept;
  void check_size() const;
  void set_size(const std::size_t& size, const std::size_t& line_size);
  // member variables
  std::size_t size_;         // cache size
  std::size_t line_size_;    // cache line size
  std::size_t items_count_;  // max cache item count
  int hit_count_;            // cache hit count
  int miss_count_;           // cache miss count
  emplace_policy policy_;    // cache emplace policy
  std::ostream& os_;         // output stream
};

// Default ctor
// Sets the items count to 1 because the line size cannot be bigger than the
// total cache size.
cache::cache()
    : size_(1),
      line_size_(1),
      items_count_(1),
      hit_count_(0),
      miss_count_(0),
      policy_(LRU),
      os_(std::cout) {}

// Explicit ctor
// Initializes the items count to size / line size
// This is necessary to know the max amount of items that the cache can hold.
// Converts the policy integer into an enum value using a static cast.
// It will check if the size and line size are powers of 2.
cache::cache(const std::size_t& size, const std::size_t& line_size,
             const int& policy, std::ostream& os)
    : size_(size),
      line_size_(line_size),
      items_count_(size / line_size_),
      hit_count_(0),
      miss_count_(0),
      policy_(static_cast<emplace_policy>(policy)),
      os_(os) {
  check_size();
}

// Returns the cache total size (Represented in bytes).
std::size_t cache::size() const noexcept { return size_; }

// Returns the cache line size (Represented in bytes).
std::size_t cache::line_size() const noexcept { return line_size_; }

// Returns the max amount of items that the cache can hold.
std::size_t cache::count() const noexcept { return items_count_; }

// Returns the amount of hits performed.
int cache::hit_count() const noexcept { return hit_count_; }

// Returns the amount of misses performed.
int cache::miss_count() const noexcept { return miss_count_; }

// Returns wheter a certain positive number is a power of two,
// that is, it can be expressed as 2^n.
// This is used to check whether the size values are valid.
// NOTE: works for n>=0, returns false for 0.
bool cache::is_pow2(const std::size_t& n) const noexcept {
  return n && !(n & (n - 1));
}

// Prints a formatted line with the current allocation attempt.
// It can output to std::cout or to an std::ofstream depending of the value
// received in the ctor.
void cache::print_line(const int& dir, const bool& hit_miss, const int& id,
                       const int& old_dir) const noexcept {
  os_.width(25);
  os_ << std::hex << dir;
  os_.width(20);
  os_ << std::dec << hit_miss;
  os_.width(10);
  os_ << id;
  os_.width(25);
  os_ << std::hex << old_dir;
  os_.width(25);
  os_ << old_dir << std::dec << '\n';
}

// Checks whether both total and line sizes are a power of 2 and that the line
// size smaller or equal to the total size.
// It is used to check the values after ctor initialization or resizing.
void cache::check_size() const {
  if (!is_pow2(size_) || !is_pow2(line_size_) || line_size_ >= size_) {
    throw std::invalid_argument("Invalid size");
  }
}

// Resizes the cache, clearing all current data and checks whether the
// parameters are valid.
void cache::set_size(const std::size_t& size, const std::size_t& line_size) {
  size_ = size;
  line_size_ = line_size;
  items_count_ = size / line_size;
  hit_count_ = 0;
  miss_count_ = 0;
  check_size();
}

}  // namespace cachesim

#endif  // CACHESIM_CACHE__H_
