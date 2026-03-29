export module level4strategy;

import chess_types;
import strategy;
import board;

export class Level4Strategy : public Strategy {
  int pieceValue(PieceType type);
  int evaluate(Board &board, Color color);

public:
  Move generateMove(Board &board, Color color) override;
};
