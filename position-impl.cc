module position;
import <string>;
import chess_types;

using namespace std;

Position::Position(int row, int col) : row{row}, col{col} {}

int Position::getRow() const { return row; }

int Position::getCol() const { return col; }

bool Position::isValid() const {
  return 0 <= row && row < 8 && 0 <= col && col < 8;
}

bool Position::operator==(const Position &other) const {
  return row == other.row && col == other.col;
}

bool Position::operator!=(const Position &other) const { return !(*this == other); }

Position Position::fromChess(const string &s) {
  if (s.length() != 2)
    return Position{};
  char file = s[0];
  char rank = s[1];
  if (file < 'a' || file > 'h')
    return Position{};
  if (rank < '1' || rank > '8')
    return Position{};
  int col = file - 'a';
  int row = 8 - (rank - '0');
  return Position{row, col};
}

string Position::toChess() const {
  string s = "a1";
  s[0] = static_cast<char>('a' + col);
  s[1] = static_cast<char>('8' - row);
  return s;
}
