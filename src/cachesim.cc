#include <cachesim/direct_cache.h>

#include <iostream>
#include <memory>

int main() {
  std::size_t cs = 16, cls = 4;
  auto e = cachesim::LRU;
  auto caches = std::make_unique<cachesim::direct_cache>(cs, cls, e);
  return 0;
}