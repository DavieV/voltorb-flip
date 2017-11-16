#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "axis.hpp"
#include "board.hpp"
#include "tile.hpp"

// Reads in the information about how many bombs and the total value stored on
// each row and column.
std::vector<std::pair<int, int>> read_axis(std::string name) {
  std::vector<std::pair<int, int>> axis_data;

  for (int i = 0; i < 5; ++i) {
    std::cout << "Enter total value and bomb count for " << name << " " << i + 1
              << std::endl;

    int t, b;
    std::cin >> t >> b;

    axis_data.push_back({t, b});
  }

  return axis_data;
}

bool board_is_solvable(Board& board, int idx, Board& solution) {
  if (idx >= 25) {
    if (board.solved()) {
      solution = board;
      return true;
    }
    return false;
  }

  if (board.invalid())
    return false;

  int i = idx / 5;
  int j = idx % 5;

  const auto& tile = board.tile_at(i, j);

  for (int k = Tile::One; k < Tile::Size; ++k) {
    if (tile.is_candidate(k)) {
      auto b = board;

      b.update_at(i, j, k);
      b.update();

      if (board_is_solvable(b, idx + 1, solution))
        return true;
    }
  }

  return false;
}

int main() {
  std::vector<Axis> rows;
  std::vector<Axis> columns;

  // Read in data for each row and column.
  auto rows_data = read_axis("row");
  auto columns_data = read_axis("column");

  Board board(rows_data, columns_data);

  board.update();

  auto start = std::chrono::system_clock::now();

  Board solution = board;
  if (board_is_solvable(board, 0, solution)) {
    std::cout << "Solution\n" << solution << std::endl;
  } else {
    std::cout << "Couldn't solve\n";
  }

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> seconds = end - start;
  std::cout << "Elapsed time: " << seconds.count() << "s\n";

  return 0;
}
