#ifndef __VOLTORB_FLIP_AXIS__
#define __VOLTORB_FLIP_AXIS__

#include <cstdint>
#include <iostream>
#include <vector>

#include "tile.hpp"

class Axis {
 public:
  Axis(int t, int b);

  Axis(const Axis& a);

  // Updates the count for the number of tiles that have been discovered along
  // the axis, as well as the total value of tiles that have been discovered
  // based on the given value.
  void update(int discovered);

  // Increment the counter for discovered bombs.
  void found_bomb();

  // Returns the total value of the undiscovered tiles along the axis.
  int undiscovered_value() const;

  // Returns the number of undiscovered tiles along the axis.
  int undiscovered_tiles() const;

  // Simple getter methods.
  int total_value() const;
  int bomb_count() const;
  int discovered_value() const;
  int discovered_tiles() const;
  int discovered_bombs() const;

  // Return the the tile indexed at |i|.
  Tile& at(int i);

 private:
  // The total value of tiles on this axis.
  int total_value_;

  // The number of bombs in this axis.
  int bomb_count_;

  // The sum of values for the tiles that have been
  // discovered along this axis.
  int discovered_value_;

  // The number of tiles that have been discovered on this axis.
  int discovered_tiles_;

  // The number of bombs that have been discovered on this axis.
  int discovered_bombs_;
};

#endif
