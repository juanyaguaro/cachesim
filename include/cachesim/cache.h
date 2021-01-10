// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_CACHE_H_
#define CACHESIM_CACHE_H_

#include <cstddef>
#include <stdexcept>

namespace cachesim {

// enum emplace_policy
enum emplace_policy { LRU, MRU };

// constant empty_space
constexpr int empty_space = -1;

// class cache
class cache {
 public:
  // ctor
  cache();
  explicit cache(const std::size_t& size, const std::size_t& line_size,
                 emplace_policy& policy);
  // dtor
  virtual ~cache() = default;
  // accessors
  std::size_t size() const noexcept;
  std::size_t line_size() const noexcept;
  virtual bool exists() const noexcept = 0;
  // mutators
  virtual void clear() = 0;
  virtual void resize(const std::size_t& size,
                      const std::size_t& line_size) = 0;
  virtual void emplace(const int& value) = 0;

 protected:
  bool is_pow2(const std::size_t& n) const noexcept;
  // member variables
  std::size_t size_;       // cache size
  std::size_t line_size_;  // cache line size
  emplace_policy policy_;  // emplace policy
};

cache::cache() : size_(1), line_size_(1), policy_(LRU) {}

cache::cache(const std::size_t& size, const std::size_t& line_size,
             emplace_policy& policy)
    : size_(size), line_size_(line_size), policy_(policy) {
  if (!is_pow2(size) || !is_pow2(line_size) || line_size > size) {
    throw std::invalid_argument("Invalid size");
  }
}

std::size_t cache::size() const noexcept { return size_; }

std::size_t cache::line_size() const noexcept { return line_size_; }

bool cache::is_pow2(const std::size_t& n) const noexcept {
  return n && !(n & (n - 1));
}

}  // namespace cachesim

#endif  // CACHESIM_CACHE_H_
