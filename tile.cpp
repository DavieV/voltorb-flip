#include "iostream"

#include "axis.hpp"
#include "tile.hpp"

Tile::Tile(Axis r, Axis c)
    : row_(r),
      column_(c),
      used_(false),
      val_(0),
      candidates_{{true, true, true, true}} {}

int Tile::val() const { return val_; }

bool Tile::used() const { return used_; };

bool Tile::bomb_candidate() const { return candidates_[Bomb]; }

bool Tile::one_candidate() const { return candidates_[One]; }

bool Tile::two_candidate() const { return candidates_[Two]; }

bool Tile::three_candidate() const { return candidates_[Three]; }

void Tile::update_candidates() {
  Tile::update_candidates(row_);
  Tile::update_candidates(column_);
}

void Tile::update_candidates(const Axis& axis) {
  if (axis.undiscovered_value() == axis.undiscovered_tiles() &&
      axis.undiscovered_value() != 0) {
    // If the number of remaining undiscovered tiles is equal to the number of
    // remaining tile value along the axis, then it is not possible for any tile
    // in the axis to be a 2 or a 3.
    candidates_[Two] = false;
    candidates_[Three] = false;
  }

  /*
  if (axis.undiscovered_value() == 0) {
    // discovered a bomb.
    // TODO: Remove this magic number stuff once I figure out how to properly
    // compare and int value with an enum type.
    for (int i = 0; i < 3; ++i) {
      candidates_[i] = false;
    }

    row_.found_bomb();
    column_.found_bomb();
  }
  */

  if (axis.discovered_bombs() == axis.bomb_count()) {
    candidates_[Bomb] = false;
  }
}

void Tile::update_value() {
  int c = 0;
  int val;
  for (int i = 0; i < 4; ++i) {
    if (candidates_[i]) {
      ++c;
      val = i + 1;
    }
  }

  if (c == 1) {
    // If there is only one candidate flag set then that is the value of the
    // tile.
    // Otherwise the value of the tile is still unknown.
    val_ = val;
    row_.update(val_);
    column_.update(val_);
  }
}

void Tile::set_candidate(int c, bool flag) { candidates_[c] = flag; }

void Tile::set_value(int val) {
  val_ = val;
  // TODO: Remove this magic number stuff eventually.
  for (int i = 0; i < 4; ++i) {
    if (i + 1 == val || val == 0) {
      // If |val| is 0 then it means the value of the tile is unkown, meaning
      // that is a candidate for all values.
      candidates_[i] = true;
    } else {
      candidates_[i] = false;
    }
  }
}

void Tile::set_used(bool val) { used_ = val; }

std::ostream& operator<<(std::ostream& os, const Tile& t) {
  os << "(";
  for (int i = Tile::One; i < Tile::Size; ++i) {
    if (i > Tile::One) {
      os << ",";
    }
    os << t.candidates_[i];
  }
  os << ")";
  return os;
}
