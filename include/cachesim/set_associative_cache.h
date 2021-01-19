// Copyright 2021 Juan Yaguaro
#ifndef CACHESIM_SET_ASSOCIATIVE_CACHE_H_
#define CACHESIM_SET_ASSOCIATIVE_CACHE_H_

#include <cachesim/cache_.h>

#include <cmath>

namespace cachesim {

// class set_associative_cache
// Represents a set-associative mapped cache.
// Inherits from cache.
class set_associative_cache final : public cache {
 public:
  // ctor
  set_associative_cache();
  explicit set_associative_cache(const std::size_t& size,
                                 const std::size_t& line_size,
                                 const int& policy, std::ostream& os,
                                 const bool& hex);
  ~set_associative_cache() = default;
  // accessors
  virtual std::size_t set_count() const noexcept;
  // mutators
  void clear() override final;
  void resize(const std::size_t& size,
              const std::size_t& line_size) override final;
  void allocate(const int& value) override final;

 private:
  std::size_t get_set_count() const noexcept;
  int get_id(const int& value) const noexcept override final;
  void replace(const int& id, const int& value);
  void replace_lru(const int& id, const int& value) noexcept;
  void replace_mru(const int& id, const int& value) noexcept;
  // member variables
  std::size_t set_count_;         // number of cache sets
  std::vector<cache_set> items_;  // cache sets of items
};

// Default ctor
// Creates a 1 set cache.
set_associative_cache::set_associative_cache()
    : cache(), set_count_(1), items_(1, cache_set()) {}

// Explicit ctor
// Creates an n sets cache.
// The sizes check is performed under the cache ctor.
set_associative_cache::set_associative_cache(const std::size_t& size,
                                             const std::size_t& line_size,
                                             const int& policy,
                                             std::ostream& os, const bool& hex)
    : cache(size, line_size, policy, os, hex),
      set_count_(get_set_count()),
      items_(set_count_, cache_set()) {}

// Returns the set count of the cache.
std::size_t set_associative_cache::set_count() const noexcept {
  return set_count_;
}

// Wipes all sets.
void set_associative_cache::clear() {
  std::for_each(items_.begin(), items_.end(), [](auto set) { set.clear(); });
  hit_count_ = 0;
  miss_count_ = 0;
}

// Resizes the cache and the sets after checking the sizes.
void set_associative_cache::resize(const std::size_t& size,
                                   const std::size_t& line_size) {
  set_size(size, line_size);
  set_count_ = get_set_count();
  items_.clear();
  items_.resize(set_count_, cache_set());
}

// Puts an element in its belonged set inside cache.
// Also prints the current allocation attempt.
// This is the main interaction function.
void set_associative_cache::allocate(const int& value) {
  auto id{get_id(value)};
  auto set_size{items_count_ / set_count_};
  auto set_it{std::find(items_[id].begin(), items_[id].end(), value)};
  auto found{set_it != std::end(items_[id])};

  print_line(value, found, id,
             (items_[id].size() < set_size ? empty_space : items_[id].back()));
  if (found) {
    items_[id].erase(set_it);
    items_[id].push_back(value);
    ++hit_count_;
  } else {
    if (items_[id].size() < set_size) {  // set has space
      items_[id].push_back(value);
    } else {  // set is full
      replace(id, value);
    }
    ++miss_count_;
  }
}

// Returns the appropiate set count based on the max amount of items in cache.
// This tries to make the cache set count as close as possible to the max amount
// of items inside a set. This means that it tries to aproximate to an n*n
// matrix but having the set count lower than the set item count.
std::size_t set_associative_cache::get_set_count() const noexcept {
  auto items_count_log2{static_cast<std::size_t>(log2(items_count_))};

  return items_count_ > 1
             ? !(items_count_log2 % 2) ? items_count_log2 : items_count_log2 - 1
             : 1;
}

// Returns the id of the set in which the new ellement should be allocated.
int set_associative_cache::get_id(const int& value) const noexcept {
  return value % set_count_;
}

// Calls the appropiate replace algorithm depending on the initial
// configuration. In case the policy is not in range, it will throw an
// exception.
void set_associative_cache::replace(const int& id, const int& value) {
  switch (policy_) {
    case LRU:
      replace_lru(id, value);
      break;
    case MRU:
      replace_mru(id, value);
      break;
    default:
      throw std::invalid_argument("No such replacement policy");
      break;
  }
}

// Replaces the least recently used value with the new value to be allocated.
void set_associative_cache::replace_lru(const int& id,
                                        const int& value) noexcept {
  items_[id].erase(items_[id].begin());
  items_[id].push_back(value);
}

// Replaces the most recently used value with the new value to be allocated.
void set_associative_cache::replace_mru(const int& id,
                                        const int& value) noexcept {
  *std::prev(items_[id].end()) = value;
}

}  // namespace cachesim

#endif  // CACHESIM_SET_ASSOCIATIVE_CACHE_H_
