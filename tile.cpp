#include "iostream"

#include "axis.hpp"
#include "tile.hpp"

Tile::Tile()
    : val_(0), used_(false), candidates_{{false, true, true, true, true}} {}

Tile::Tile(const Tile &t)
    : val_(t.val_), used_(t.used_), candidates_(t.candidates_) {}

int Tile::val() const { return val_; }

bool Tile::used() const { return used_; }

bool Tile::bomb_candidate() const { return candidates_[Bomb]; }

bool Tile::one_candidate() const { return candidates_[One]; }

bool Tile::two_candidate() const { return candidates_[Two]; }

bool Tile::three_candidate() const { return candidates_[Three]; }

bool Tile::is_candidate(int type) const { return candidates_[type]; }

void Tile::set_candidate(int c, bool flag) { candidates_[c] = flag; }

void Tile::set_value(int val) { val_ = val; }

void Tile::set_used(bool val) { used_ = val; }

std::ostream& operator<<(std::ostream& os, const Tile& t) {
  os << "(";
  for (int i = Tile::One; i < Tile::Size; ++i) {
    if (i > Tile::One) {
      os << ",";
    }
    os << t.candidates_[i];
  }
  os << "," << t.val() << ")";
  return os;
}
