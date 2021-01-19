// Copyright 2020 Juan Yaguaro
#ifndef CACHESIM_SET_ASSOCIATIVE_CACHE_H_
#define CACHESIM_SET_ASSOCIATIVE_CACHE_H_

#include <cachesim/cache_.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace cachesim {

// class set_associative_cache
class set_associative_cache final : public cache {
  using cache_set = std::vector<int>;

 public:
  // ctor
  set_associative_cache();
  explicit set_associative_cache(const std::size_t& size,
                                 const std::size_t& line_size,
                                 const int& policy, std::ostream& os);
  ~set_associative_cache() = default;
  // accessors
  virtual std::size_t set_count() const noexcept;
  // mutators
  void clear() override final;
  void resize(const std::size_t& size,
              const std::size_t& line_size) override final;
  void emplace(const int& value) override final;

 private:
  std::size_t get_set_count(const std::size_t& size,
                            const std::size_t& line_size) const noexcept;
  int get_id(const int& value) const noexcept override final;
  void replace(const int& id, const int& value);
  void emplace_lru(const int& id, const int& value) noexcept;
  void emplace_mru(const int& id, const int& value) noexcept;
  // member variables
  std::size_t set_count_;
  std::vector<cache_set> items_;
};

set_associative_cache::set_associative_cache()
    : cache(), set_count_(1), items_(1, cache_set()) {}

set_associative_cache::set_associative_cache(const std::size_t& size,
                                             const std::size_t& line_size,
                                             const int& policy,
                                             std::ostream& os)
    : cache(size, line_size, policy, os),
      set_count_(get_set_count(size, line_size)),
      items_(set_count_, cache_set()) {}

std::size_t set_associative_cache::set_count() const noexcept {
  return set_count_;
}

void set_associative_cache::clear() {
  std::for_each(items_.begin(), items_.end(), [](auto set) { set.clear(); });
  hit_count_ = 0;
  miss_count_ = 0;
}

void set_associative_cache::resize(const std::size_t& size,
                                   const std::size_t& line_size) {
  set_size(size, line_size);
  set_count_ = get_set_count(size, line_size);
  items_.clear();
  items_.resize(set_count_, cache_set());
}

void set_associative_cache::emplace(const int& value) {
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
    if (items_[id].size() < set_size) {
      items_[id].push_back(value);
    } else {
      replace(id, value);
    }
    ++miss_count_;
  }
}

std::size_t set_associative_cache::get_set_count(
    const std::size_t& size, const std::size_t& line_size) const noexcept {
  auto items_count_log2{(std::size_t)log2(items_count_)};

  return items_count_ > 1
             ? !(items_count_log2 % 2) ? items_count_log2 : items_count_log2 - 1
             : 1;
}

int set_associative_cache::get_id(const int& value) const noexcept {
  return value % set_count_;
}

void set_associative_cache::replace(const int& id, const int& value) {
  switch (policy_) {
    case LRU:
      emplace_lru(id, value);
      break;
    case MRU:
      emplace_mru(id, value);
      break;
    default:
      throw std::invalid_argument("No such replacement policy");
      break;
  }
}

void set_associative_cache::emplace_lru(const int& id,
                                        const int& value) noexcept {
  items_[id].erase(items_[id].begin());
  items_[id].push_back(value);
}

void set_associative_cache::emplace_mru(const int& id,
                                        const int& value) noexcept {
  *std::prev(items_[id].end()) = value;
}

}  // namespace cachesim

#endif  // CACHESIM_SET_ASSOCIATIVE_CACHE_H_
