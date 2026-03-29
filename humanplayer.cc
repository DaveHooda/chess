export module humanplayer;

import player;
import move;
import chess_types;
import board;

export class HumanPlayer : public Player {
  Move nextMove;

public:
  HumanPlayer(Color color);
  bool isHuman() const override;
  void setMove(const Move &move);
  Move getMove(Board &) override;
};
