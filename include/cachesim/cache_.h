// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_CACHE__H_
#define CACHESIM_CACHE__H_

#include <cstddef>
#include <iomanip>
#include <iostream>
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
  std::size_t size_;       // cache size
  std::size_t line_size_;  // cache line size
  std::size_t items_count_;
  int hit_count_;
  int miss_count_;
  emplace_policy policy_;  // emplace policy
  std::ostream& os_;
};

cache::cache()
    : size_(1),
      line_size_(1),
      items_count_(1),
      hit_count_(0),
      miss_count_(0),
      policy_(LRU),
      os_(std::cout) {}

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

std::size_t cache::size() const noexcept { return size_; }

std::size_t cache::line_size() const noexcept { return line_size_; }

std::size_t cache::count() const noexcept { return items_count_; }

int cache::hit_count() const noexcept { return hit_count_; }

int cache::miss_count() const noexcept { return miss_count_; }

bool cache::is_pow2(const std::size_t& n) const noexcept {
  return n && !(n & (n - 1));
}

void cache::print_line(const int& dir, const bool& hit_miss, const int& id,
                       const int& old_dir) const noexcept {
  os_.width(25);
  os_ << dir;
  os_.width(20);
  os_ << hit_miss;
  os_.width(10);
  os_ << id;
  os_.width(25);
  os_ << old_dir;
  os_.width(25);
  os_ << dir << '\n';
}

void cache::check_size() const {
  if (!is_pow2(size_) || !is_pow2(line_size_) || line_size_ > size_) {
    throw std::invalid_argument("Invalid size");
  }
}

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
