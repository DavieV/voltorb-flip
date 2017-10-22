#include "axis.hpp"
#include <cstdint>

Axis::Axis(int t, int b)
    : total_value_(t),
      bomb_count_(b),
      discovered_value_(0),
      discovered_tiles_(0),
      discovered_bombs_(0) {}

void Axis::update(int discovered) {
  if (discovered == 4) {
    found_bomb();
    return;
  }
  ++discovered_tiles_;
  discovered_value_ += discovered;
}

void Axis::found_bomb() { ++discovered_bombs_; }

int Axis::undiscovered_value() const {
  return total_value_ - discovered_value_;
}

int Axis::undiscovered_tiles() const {
  return 5 - bomb_count_ - discovered_tiles_;
}
int Axis::total_value() const { return total_value_; }

int Axis::bomb_count() const { return bomb_count_; }

int Axis::discovered_value() const { return discovered_value_; }

int Axis::discovered_tiles() const { return discovered_tiles_; }

int Axis::discovered_bombs() const { return discovered_bombs_; }
