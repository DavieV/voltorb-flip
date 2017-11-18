#include <vector>
#include <list>
#include "axis.hpp"
#include "board.hpp"
#include "tile.hpp"

Board::Board(const std::vector<std::pair<int, int>> &row_data,
             const std::vector<std::pair<int, int>> &column_data)
    : tiles_(5, std::vector<Tile>(5)) {
  for (int i = 0; i < 5; ++i) {
    const auto &r = row_data[i];
    const auto &c = column_data[i];
    rows_.emplace_back(r.first, r.second);
    columns_.emplace_back(c.first, c.second);
  }
}

Tile& Board::tile_at(int i, int j) { return tiles_[i][j]; }
Axis& Board::row_at(int i) { return rows_[i]; }
Axis& Board::column_at(int i) { return columns_[i]; }

bool Board::invalid() const {
  for (int i = 0; i < 5; ++i) {
    int row_val_total = 0;
    int column_val_total = 0;
    int row_bomb_total = 0;
    int column_bomb_total = 0;

    for (int j = 0; j < 5; ++j) {
      int row_val = tiles_[i][j].val();
      int column_val = tiles_[j][i].val();

      if (Tile::One <= row_val && row_val <= Tile::Three)
        row_val_total += row_val;

      if (Tile::One <= column_val && column_val <= Tile::Three)
        column_val_total += column_val;

      if (row_val == Tile::Bomb)
        ++row_bomb_total;

      if (column_val == Tile::Bomb)
        ++column_bomb_total;


    }
    if (row_val_total > rows_[i].total_value() ||
        column_val_total > columns_[i].total_value())
      return true;

    if (row_bomb_total > rows_[i].bomb_count() ||
        column_bomb_total > columns_[i].bomb_count())
      return true;
  }

  return false;
}

bool Board::solved() const {
  for (int i = 0; i < 5; ++i) {
    int row_val_total = 0;
    int column_val_total = 0;
    int row_bomb_total = 0;
    int column_bomb_total = 0;

    for (int j = 0; j < 5; ++j) {
      int row_val = tiles_[i][j].val();
      int column_val = tiles_[j][i].val();

      if (Tile::One <= row_val && row_val <= Tile::Three) {
        row_val_total += row_val;
      }

      if (Tile::One <= column_val && column_val <= Tile::Three) {
        column_val_total += column_val;
      }

      if (row_val == Tile::Bomb) {
        ++row_bomb_total;
      }

      if (column_val == Tile::Bomb) {
        ++column_bomb_total;
      }
    }

    if (row_val_total != rows_[i].total_value() ||
        column_val_total != columns_[i].total_value())
      return false;

    if (row_bomb_total != rows_[i].bomb_count() ||
        column_bomb_total != columns_[i].bomb_count())
      return false;
  }

  return true;
}

void Board::update_at(int i, int j, int val) {
  tiles_[i][j].set_value(val);
  rows_[i].update(val);
  columns_[j].update(val);
}

void Board::update() {
  update_tile_candidates();
  // update_tile_values();
}

void Board::update_tile_candidates() {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      Tile &t = tiles_[i][j];

      int row_uv = rows_[i].undiscovered_value();
      int row_ut = rows_[i].undiscovered_tiles();
      int column_uv = columns_[j].undiscovered_value();
      int column_ut = columns_[j].undiscovered_tiles();

      if ((row_uv == row_ut && row_uv != 0) ||
          (column_uv == column_ut && column_uv != 0)) {
        t.set_candidate(Tile::Two, false);
        t.set_candidate(Tile::Three, false);
      }

      // Check to see if it is impossible for One to be a candidate value.
      for (int k = 1; k <= 5; ++k) {
        if ((row_ut == k && row_uv == (k * 2) + k - 1) ||
            (column_ut == k && column_uv == (k * 2) + k - 1)) {
          t.set_candidate(Tile::One, false);
        }
      }

      if ((rows_[i].discovered_bombs() == rows_[i].bomb_count()) ||
          (columns_[j].discovered_bombs() == columns_[j].bomb_count())) {
        t.set_candidate(Tile::Bomb, false);
      }
    }
  }
}

void Board::update_tile_values() {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      int c = 0;
      int val;
      for (int k = Tile::One; k < Tile::Size; ++k) {
        ++c;
        val = i + 1;
      }

      if (c == 1) {
        // If there is only one candidate flag set then that is the value of
        // tile. Otherwise the value of the tile is still unknown.
        tiles_[i][j].set_value(val);
        rows_[i].update(val);
        columns_[j].update(val);
      }

      if (c == 0) {
        // We've encountered an error as the file is not a candidate for any
        // value.
        tiles_[i][j].set_value(-1);
      }
    }
  }
}

void Board::reset_candidates() {
  for (int i = 0; i < 5; ++i) {
    rows_[i].reset();
    columns_[i].reset();
  }

  for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
      for (int k = Tile::One; k < Tile::Size; ++k) {
        auto& tile = tiles_[i][j];
        if (tile.val() != 0 && !tile.used()) {
          tile.set_candidate(k, false);
          tile.set_used(true);
          rows_[i].update(k);
          columns_[j].update(k);
        } else {
          tile.set_candidate(k, true);
        }
      }
    }
  }
}

void Board::intersect(const Board& b) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (tiles_[i][j].val() != b.tiles_[i][j].val())
        tiles_[i][j].set_value(Tile::Unknown);
    }
  }
}

void Board::safe_intersect(const Board& b) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (tiles_[i][j].val() != Tile::Bomb && b.tiles_[i][j].val() != Tile::Bomb)
        tiles_[i][j].set_value(Tile::Safe);
      else
        tiles_[i][j].set_value(Tile::Unknown);
    }
  }
}

bool Board::known_tiles_match(const Board &b) const {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (tiles_[i][j].val() != Tile::Unknown &&
          tiles_[i][j].val() != b.tiles_[i][j].val())
        return false;
    }
  }
  return true;
}

std::ostream &operator<<(std::ostream &os, const Board &b) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      os << b.tiles_[i][j];
    }
    os << std::endl;
  }
  return os;
}
