#ifndef __VOLTORB_FLIP_TILE__
#define __VOLTORB_FLIP_TILE__

#include <array>
#include <iostream>
#include <memory>
#include "axis.hpp"

class Tile {
 public:
  Tile();

  Tile(const Tile& t);

  enum {
    Unknown = 0,
    One = 1,
    Two = 2,
    Three = 3,
    Bomb = 4,
    Size = 5,
    Safe = 6
  };

  // Simple getter methods.
  int val() const;
  bool used() const;
  bool bomb_candidate() const;
  bool one_candidate() const;
  bool two_candidate() const;
  bool three_candidate() const;
  bool is_candidate(int type) const;

  // Updates the candidates array at the given candidate |c| to the value
  // of |flag|.
  void set_candidate(int c, bool flag);

  // Updates the tile's value to be the given |val|, and appropiately updates
  // the candidates array.
  void set_value(int val);

  void set_used(bool val);

  friend std::ostream& operator<<(std::ostream& os, const Tile& t);

 private:
  // The value of the tile, 0 represents an unknown value. 4 represents a bomb.
  int val_;

  bool used_;

  // Array of candidates flags which represents whether or not the tile may
  // possible hold each value. The index of the array using the Candidate enum
  // corresponds to the candidate for that value.
  // NOTE: In order to '1-index' the array there is an empty element at index 0.
  std::array<bool, Size> candidates_;
};

#endif
