// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_CACHE_H_
#define CACHESIM_CACHE_H_

#include <cstddef>

namespace cachesim {

// enum emplace_policy
enum emplace_policy { LRU, MRU };

// class cache
class cache {
 public:
  // ctor
  cache() = default;
  explicit cache(const std::size_t& size, const std::size_t& line_size,
                 emplace_policy& policy);
  // dtor
  ~cache() = default;
  // accessors
  virtual std::size_t size() const noexcept = 0;
  virtual std::size_t line_size() const noexcept = 0;
  virtual bool exists() const noexcept = 0;
  // mutators
  virtual void resize(const std::size_t& size,
                      const std::size_t& line_size) = 0;
  virtual void emplace(const int& value) = 0;

 protected:
  std::size_t size_;       // cache size
  std::size_t line_size_;  // cache line size
  emplace_policy policy_;  // emplace policy
};

cache::cache(const std::size_t& size, const std::size_t& line_size,
             emplace_policy& policy)
    : size_(size), line_size_(line_size), policy_(policy) {}

}  // namespace cachesim

#endif  // CACHESIM_CACHE_H_
