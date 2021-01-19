// Copyright 2021 Juan Yaguaro
#ifndef CACHESIM_DIRECT_CACHE_H_
#define CACHESIM_DIRECT_CACHE_H_

#include <cachesim/cache_.h>

namespace cachesim {

// class direct_cache
// Represents a direct-mapped cache.
// Inherits from cache.
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
  void allocate(const int& value) override final;

 private:
  int get_id(const int& value) const noexcept override final;
  // member variables
  cache_set items_;  // cache items
};

// Default ctor
// Creates a 1 item cache filled with an empty space.
direct_cache::direct_cache() : cache(), items_(1, empty_space) {}

// Explicit ctor
// Creates an n item cache filled with empty spaces.
// The sizes check is performed under the cache ctor.
direct_cache::direct_cache(const std::size_t& size,
                           const std::size_t& line_size, const int& policy,
                           std::ostream& os)
    : cache(size, line_size, policy, os), items_(items_count_, empty_space) {}

// Wipes all items and replaces it with empty spaces.
void direct_cache::clear() {
  std::fill(items_.begin(), items_.end(), empty_space);
}

// Resizes the cache and the vector after checking the sizes.
void direct_cache::resize(const std::size_t& size,
                          const std::size_t& line_size) {
  set_size(size, line_size);
  clear();
  items_.resize(size / line_size, empty_space);
}

// Puts an element in its belonged place inside cache.
// Also prints the current allocation attempt.
// This is the main interaction function.
void direct_cache::allocate(const int& value) {
  auto id{get_id(value)};
  auto found{items_[id] == value};

  print_line(value, found, id, items_[id]);
  if (found) {
    ++hit_count_;
  } else {
    items_[id] = value;
    ++miss_count_;
  }
}

// Returns the id of the position in which the new ellement should be allocated.
int direct_cache::get_id(const int& value) const noexcept {
  return value % (items_count_);
}

}  // namespace cachesim

#endif  // CACHESIM_DIRECT_CACHE_H_
