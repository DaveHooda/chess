export module computerplayer;

import <memory>;
import player;
import move;
import board;
import strategy;
import chess_types;

export class ComputerPlayer : public Player {
  std::unique_ptr<Strategy> strategy;

public:
  ComputerPlayer(Color color, int level);
  bool isHuman() const override;
  Move getMove(Board &board) override;
};
