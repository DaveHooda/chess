export module position;

import <string>;
import chess_types;

export class Position {
  int row;
  int col;

public:
  Position(int row = -1, int col = -1);

  int getRow() const;
  int getCol() const;

  bool isValid() const;

  bool operator==(const Position &other) const;
  bool operator!=(const Position &other) const;

  static Position fromChess(const std::string &s);
  std::string toChess() const;
};
