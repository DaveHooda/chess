export module strategy;

import chess_types;
import move;
import board;

export class Strategy {
public:
  virtual ~Strategy();
  virtual Move generateMove(Board &board, Color color) = 0;
};
