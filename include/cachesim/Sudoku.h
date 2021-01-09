// Copyright 2020 Juan Yaguaro
#ifndef SUDOKUPP_SUDOKU_H_
#define SUDOKUPP_SUDOKU_H_

#include <algorithm>
#include <ostream>

#include <array>
#include <tuple>
#include <vector>

namespace sudokupp {

inline constexpr unsigned grid_size = 81;

using Matrix = std::array<unsigned, grid_size>;
using Cell = std::tuple<unsigned, unsigned, unsigned>;

// Class Sudoku
class Sudoku {
 public:
  // Constructors
  Sudoku() = default;
  Sudoku(const Sudoku& other);
  explicit Sudoku(const std::vector<Cell>& celss);

  // Destructor
  ~Sudoku() = default;

  // Operators

  // Const member functions
  bool empty() const;
  bool solved() const;
  bool valid() const;

  // Non-const member functions
  void load(const Matrix& grid);
  void load(const std::vector<Cell>& cells);
  void solve();
  void clear();

 private:
 // Const member functions
  bool valid_range(const unsigned& n, const unsigned& i, const unsigned& j) const;
  bool valid_row(const unsigned& n, const unsigned& i) const;
  bool valid_column(const unsigned& n, const unsigned& j) const;
  bool valid_block(const unsigned& n, const unsigned& i, const unsigned& j) const;

  Matrix grid_;
};

// Public member functions

// Constructors

Sudoku::Sudoku(const Sudoku& other) : grid_(Matrix{})
{
  load(other.grid_);
}

Sudoku::Sudoku(const std::vector<Cell>& cells) : grid_(Matrix{})
{
  load(cells);
}

// Operators

// Const member functions

bool Sudoku::empty() const
{
  return std::all_of(grid_.begin(), grid_.end(),
    [](const unsigned& n) {
      return n == 0;
    });
}

bool Sudoku::solved() const
{
  return std::all_of(grid_.begin(), grid_.end(),
    [this, p = 0](const unsigned& n) mutable {
      const unsigned j = p % 9;
      const unsigned i = (p - j) / 9;
      const bool valid = valid_range(n, 1, 9) && valid_row(n, i)
        && valid_column(grid_[p], j) && valid_block(grid_[p], i, j);
      
      ++p;
      return valid;
    });
}

bool Sudoku::valid() const
{
  return std::all_of(grid_.begin(), grid_.end(),
    [this, p = 0](const unsigned& n) mutable {
      const unsigned j = p % 9;
      const unsigned i = (p - j) / 9;
      const bool valid = valid_range(n, 0, 9) && valid_row(n, i)
        && valid_column(grid_[p], j) && valid_block(grid_[p], i, j);
      
      ++p;
      return valid;
    });
}

// Non-const member functions

void Sudoku::load(const Matrix& grid)
{
  grid_ = grid;
}

void Sudoku::load(const std::vector<Cell>& cells)
{
  std::for_each(cells.begin(), cells.end(),
    [&grid_ = grid_](const Cell& cell) {
      grid_[std::get<0>(cell) * 9 + std::get<1>(cell)] = std::get<2>(cell);
    });
}

void Sudoku::solve();

void Sudoku::clear()
{
  grid_.fill(0);
}

// Private member functions

// Const member functions

bool Sudoku::valid_range(const unsigned& n, const unsigned& i, const unsigned& j) const
{
  return n >= i && n <= j;
}

bool Sudoku::valid_row(const unsigned& n, const unsigned& i) const
{
  unsigned count = 0;

  for (unsigned it = i * 9; it < (i * 9) + 9; ++it) {
    if (grid_[it] == n) {
      ++count;
    }
  }

  return count == 1;
}

bool Sudoku::valid_column(const unsigned& n, const unsigned& j) const
{
  unsigned count = 0;

  for (unsigned it = j; it < grid_size; it += 9) {
    if (grid_[it] == n) {
      ++count;
    }
  }

  return count == 1;
}

bool Sudoku::valid_block(const unsigned& n, const unsigned& i, const unsigned& j) const
{
  unsigned count = 0;

  for (unsigned it = i - (i % 3); it < it + 3; ++it) {
    for (unsigned jt = j - (j % 3); jt < jt + 3; ++jt) {
      if (grid_[it * 9 + jt] == n) {
        ++count;
      }
    }
  }

  return count == 1;
}

}  // namespace sudokupp

#endif  // SUDOKUPP_SUDOKU_H_
