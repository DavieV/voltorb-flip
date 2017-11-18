#include <chrono>
#include <cstdint>
#include <iostream>
#include <list>
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

void find_solutions(Board& board, int idx, std::list<Board>& solutions) {
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
    if (!tile.is_candidate(k)) continue;

    auto b = board;

    b.update_at(i, j, k);
    b.update();

    find_solutions(b, idx + 1, solutions);
  }
}

void intersect_solutions(std::list<Board>& solutions, Board& b) {
  if (solutions.empty())
    return;

  for (const Board& solution : solutions)
    b.safe_intersect(solution);
}

void discover_safe_tiles(Board& b) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      Tile& tile = b.tile_at(i, j);
      if (tile.val() == Tile::Safe) {
        std::cout << "Enter the value for tile at row " << i + 1 << " column "
                  << j + 1 << " : ";
        int value;
        std::cin >> value;
        tile.set_value(value);
      }
    }
  }
}

void discard_solutions(std::list<Board>& solutions, const Board &b) {
  for (auto iter = solutions.begin(); iter != solutions.end(); ) {
    if (!b.known_tiles_match(*iter))
      iter = solutions.erase(iter);
    else
      ++iter;
  }
}

void find_solution(Board& board) {
  std::list<Board> solutions;
  find_solutions(board, 0, solutions);

  while (solutions.size() > 1) {
    std::cout << "Number of solutions: " << solutions.size() << std::endl;
    intersect_solutions(solutions, board);
    std::cout << board << std::endl;
    discover_safe_tiles(board);
    std::cout << board << std::endl;
    discard_solutions(solutions, board);
  }
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

  find_solution(board);

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> seconds = end - start;

  std::cout << "Elapsed time: " << seconds.count() << "s\n";

  return 0;
}
