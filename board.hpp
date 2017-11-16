#ifndef __VOLTORB_FLIP_BOARD__
#define __VOLTORB_FLIP_BOARD__

#include <iostream>
#include <vector>

#include "axis.hpp"
#include "tile.hpp"

class Board {
 public:
   Board(const std::vector<std::pair<int, int>>& row_data,
         const std::vector<std::pair<int, int>>& column_data);

   void update();

   // Index the board at row |i|, column |j|.
   Tile& tile_at(int i, int j);

   // Get the row/column at index |i|.
   Axis& row_at(int i);
   Axis& column_at(int i);

   // Updates the tile at |(i, j)| as well as it's corresponding row/column with
   // the provided |val|.
   void update_at(int i, int j, int val);

   // Determine if the current values assigned on the board break any of the
   // rules outlined by the axes.
   bool invalid();

   // Determines whether or not the current board is solved based on on the
   // rules for each axis and the values assigned for each tile. Note that this
   // function assumes that every tile on the board has been assigned a value.
   bool solved();

   // Simply reset the candidate flags for each tile to true.
   void reset_candidates();

   friend std::ostream& operator<<(std::ostream& os, const Board& b);

 private:
  void update_tile_candidates();
  void update_tile_values();

  std::vector<Axis> rows_;
  std::vector<Axis> columns_;

  std::vector<std::vector<Tile>> tiles_;
};

#endif
