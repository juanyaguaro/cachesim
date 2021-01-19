// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_DIRECT_CACHE_H_
#define CACHESIM_DIRECT_CACHE_H_

#include <cachesim/cache_.h>

#include <algorithm>
#include <iostream>
#include <vector>

namespace cachesim {

// class direct_cache
class direct_cache final : public cache {
 public:
  // ctor
  direct_cache();
  explicit direct_cache(const std::size_t& size, const std::size_t& line_size,
                        const int& policy, std::ostream& os);
  ~direct_cache() = default;
  // mutators
  void clear() override final;
  void resize(const std::size_t& size,
              const std::size_t& line_size) override final;
  void emplace(const int& value) override final;

 private:
  int get_id(const int& value) const noexcept override final;
  // member variables
  std::vector<int> items_;
};

direct_cache::direct_cache() : cache(), items_(1, empty_space) {}

direct_cache::direct_cache(const std::size_t& size,
                           const std::size_t& line_size, const int& policy,
                           std::ostream& os)
    : cache(size, line_size, policy, os),
      items_(size / line_size, empty_space) {}

void direct_cache::clear() {
  std::fill(items_.begin(), items_.end(), empty_space);
}

void direct_cache::resize(const std::size_t& size,
                          const std::size_t& line_size) {
  set_size(size, line_size);
  clear();
  items_.resize(size / line_size, empty_space);
}

void direct_cache::emplace(const int& value) {
  auto id{get_id(value)};

  os_ << value << '\t' << (items_[id] == value) << '\t' << items_[id] << '\t'
      << value << '\n';
  if (items_[id] == value) {
    ++hit_count_;
  } else {
    items_[id] = value;
    ++miss_count_;
  }
}

int direct_cache::get_id(const int& value) const noexcept {
  return value % (size_ / line_size_);
}

}  // namespace cachesim

#endif  // CACHESIM_DIRECT_CACHE_H_
