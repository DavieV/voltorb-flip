#ifndef __VOLTORB_FLIP_TILE__
#define __VOLTORB_FLIP_TILE__

#include <array>
#include <iostream>
#include <memory>
#include "axis.hpp"

class Tile {
 public:
  Tile(Axis r, Axis c);

  enum { One = 0, Two = 1, Three = 2, Bomb = 3, Size = 4 };

  // Simple getter methods.
  bool used() const;
  int val() const;
  bool bomb_candidate() const;
  bool one_candidate() const;
  bool two_candidate() const;
  bool three_candidate() const;

  void update_candidates();

  // Uses the information from the given axis which corresponds to this tile to
  // determine whether it is definitive that this tile does not hold anything of
  // value. In such a case the |two_candidate_| and |three_candidate_| variables
  // are updated.
  void update_candidates(const Axis& axis);

  // Uses the information from the candidates array to see if this tile has a
  // definitive value.
  void update_value();

  // Updates the candidates array at the given candidate |c| to the value
  // of |flag|.
  void set_candidate(int c, bool flag);

  // Updates the tile's value to be the given |val|, and appropiately updates
  // the candidates array.
  void set_value(int val);

  void set_used(bool val);

  friend std::ostream& operator<<(std::ostream& os, const Tile& t);

 private:
  // References to the row and column axes that the tile belongs to.
  Axis row_;
  Axis column_;

  bool used_;

  // The value of the tile, 0 represents an unknown value. 4 represents a bomb.
  int val_;

  // Array of candidates flags which represents whether or not the tile may
  // possible hold each value. The index of the array using the Candidate enum
  // corresponds to the candidate for that value.
  std::array<bool, Size> candidates_;
};

#endif
