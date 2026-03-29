export module level1strategy;

import strategy;
import board;
import move;
import chess_types;

export class Level1Strategy : public Strategy {
public:
  Move generateMove(Board &board, Color color) override;
};
