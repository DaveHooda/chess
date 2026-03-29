export module player;

import chess_types;
import move;
import board;

export class Player {
  Color color;

public:
  Player(Color color);
  virtual ~Player();
  Color getColor() const;
  virtual bool isHuman() const = 0;
  virtual Move getMove(Board &board) = 0;
};
