export module level3strategy;

import strategy;
import board;
import move;
import chess_types;

export class Level3Strategy : public Strategy {
public:
  Move generateMove(Board &board, Color color) override;
};
