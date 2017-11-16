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

void find_solutions(Board& board, int idx, std::vector<Board>& solutions) {
  if (idx >= 25) {
    if (board.solved()) {
      solutions.push_back(board);
    }
    return;
  }

  if (board.invalid())
    return;

  int i = idx / 5;
  int j = idx % 5;

  const auto& tile = board.tile_at(i, j);

  for (int k = Tile::One; k < Tile::Size; ++k) {
    if (tile.is_candidate(k)) {
      auto b = board;

      b.update_at(i, j, k);
      b.update();

      find_solutions(b, idx + 1, solutions);
    }
  }
}

Board intersection(Board& a, Board& b) {
  Board ret = a;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      int val = a.tile_at(i, j).val();
      if (val == b.tile_at(i, j).val())
        ret.tile_at(i, j).set_value(val);
      else
        ret.tile_at(i, j).set_value(0);
    }
  }

  return ret;
}

bool intersect_solutions(std::vector<Board>& solutions, Board& b) {
  if (solutions.empty())
    return false;

  b = solutions[0];
  for (unsigned i = 1; i < solutions.size(); ++i)
    b = intersection(b, solutions[i]);

  return true;
}

bool solve_board(Board& board) {
  std::vector<Board> solutions;
  while (!board.solved()) {
    find_solutions(board, 0, solutions);
    std::cout << solutions.size() << std::endl;
    if (!intersect_solutions(solutions, board))
      return false;
    board.reset_candidates();
    board.update();
    std::cout << board << std::endl;
    solutions.clear();
  }
  return true;
}

int main() {
  std::vector<Axis> rows;
  std::vector<Axis> columns;

  // Read in data for each row and column.
  auto rows_data = read_axis("row");
  auto columns_data = read_axis("column");

  Board board(rows_data, columns_data);

  board.update();

  // auto start = std::chrono::system_clock::now();

  if (solve_board(board))
    std::cout << board << std::endl;
  /*
  std::vector<Board> solutions;
  find_solutions(board, 0, solutions);

  std::cout << solutions.size() << std::endl;
  std::cout << intersect_solutions(solutions) << std::endl;
  */

  // auto end = std::chrono::system_clock::now();
  // std::chrono::duration<double> seconds = end - start;

  // std::cout << "Elapsed time: " << seconds.count() << "s\n";

  return 0;
}
