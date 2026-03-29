export module level2strategy;

import strategy;
import board;
import move;
import chess_types;

export class Level2Strategy : public Strategy {
public:
  Move generateMove(Board &board, Color color) override;
};
