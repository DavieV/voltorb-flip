#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "axis.hpp"
#include "tile.hpp"

// Reads in the information about how many bombs and the total value stored on
// each row and column.
void read_axis(std::vector<Axis>& axis, std::string name) {
  for (int i = 0; i < 5; ++i) {
    std::cout << "Enter total value and bomb count for " << name << " " << i + 1
              << std::endl;

    int t, b;
    std::cin >> t >> b;

    axis.emplace_back(t, b);
  }
}

void update_board(std::vector<std::vector<Tile>>& board) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      board[i][j].update_value();
      board[i][j].update_candidates();
    }
  }
}

void display_board(const std::vector<std::vector<Tile>>& board) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      std::cout << board[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

bool board_is_solvable(std::vector<std::vector<Tile>>& board) {
  // display_board(board);
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (board[i][j].val() != 0 && !board[i][j].used()) {
        return true;
        // continue;
      }

      if (!board[i][j].one_candidate() && !board[i][j].two_candidate() &&
          !board[i][j].three_candidate() && !board[i][j].bomb_candidate()) {
        // We've found a tile which is not a candidate for any value. This means
        // that the given board is not solveable.
        return false;
      }

      if (board[i][j].one_candidate()) {
        auto b = board;
        b[i][j].set_value(1);
        if (!board_is_solvable(b)) {
          board[i][j].set_candidate(Tile::One, false);
        }
      }

      if (board[i][j].two_candidate()) {
        auto b = board;
        b[i][j].set_value(2);
        if (!board_is_solvable(b)) {
          board[i][j].set_candidate(Tile::Two, false);
        }
      }

      if (board[i][j].three_candidate()) {
        auto b = board;
        b[i][j].set_value(3);
        if (!board_is_solvable(b)) {
          board[i][j].set_candidate(Tile::Three, false);
        }
      }
      if (board[i][j].bomb_candidate()) {
        auto b = board;
        b[i][j].set_value(4);
        if (!board_is_solvable(b)) {
          board[i][j].set_candidate(Tile::Bomb, false);
        }
      }

      update_board(board);
    }
  }

  return true;
}

std::pair<int, int> find_move(std::vector<std::vector<Tile>>& board) {
  auto b = board;
  if (board_is_solvable(b)) {
    board = b;
  }

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (board[i][j].val() != 0 && !board[i][j].used()) {
        return {i, j};
      }
    }
  }

  return {-1, -1};
}

int main() {
  std::vector<Axis> rows;
  std::vector<Axis> columns;

  // Read in data for each row and column.
  read_axis(rows, "row");
  read_axis(columns, "column");

  std::vector<std::vector<Tile>> board;

  for (int i = 0; i < 5; ++i) {
    board.emplace_back();
    for (int j = 0; j < 5; ++j) {
      board[i].emplace_back(rows[i], columns[j]);
    }
  }

  update_board(board);

  auto p = find_move(board);

  while (p.first != -1 && p.second != -1) {
    std::cout << "Position: (" << p.first << "," << p.second << ")\n"
              << "Value: " << board[p.first][p.second] << "\n";
    std::cout << "Making move...\n";
    board[p.first][p.second].set_used(true);
    p = find_move(board);
  }

  display_board(board);

  return 0;
}
