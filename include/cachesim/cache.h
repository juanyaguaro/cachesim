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
  void check_size() const;
  void set_size(const std::size_t& size, const std::size_t& line_size);
  // member variables
  std::size_t size_;       // cache size
  std::size_t line_size_;  // cache line size
  int hit_count_;
  int miss_count_;
  emplace_policy policy_;  // emplace policy
};

cache::cache()
    : size_(1), line_size_(1), hit_count_(0), miss_count_(0), policy_(LRU) {}

cache::cache(const std::size_t& size, const std::size_t& line_size,
             emplace_policy& policy)
    : size_(size),
      line_size_(line_size),
      hit_count_(0),
      miss_count_(0),
      policy_(policy) {
  check_size();
}

std::size_t cache::size() const noexcept { return size_; }

std::size_t cache::line_size() const noexcept { return line_size_; }

int cache::hit_count() const noexcept { return hit_count_; }

int cache::miss_count() const noexcept { return miss_count_; }

bool cache::is_pow2(const std::size_t& n) const noexcept {
  return n && !(n & (n - 1));
}

void cache::check_size() const {
  if (!is_pow2(size_) || !is_pow2(line_size_) || line_size_ > size_) {
    throw std::invalid_argument("Invalid size");
  }
}

void cache::set_size(const std::size_t& size, const std::size_t& line_size) {
  size_ = size;
  line_size_ = line_size;
  hit_count_ = 0;
  miss_count_ = 0;
  check_size();
}

}  // namespace cachesim

#endif  // CACHESIM_CACHE_H_
