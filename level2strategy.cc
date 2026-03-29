export module level2strategy;

import strategy;
import board;

export class Level2Strategy : public Strategy {
public:
  Move generateMove(Board &board, Color color) override;
};
