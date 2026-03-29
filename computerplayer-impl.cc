module computerplayer;
import <memory>;
import player;
import strategy;
import level1strategy;
import level2strategy;
import level3strategy;
import level4strategy;
import board;

using namespace std;

ComputerPlayer::ComputerPlayer(Color color, int level) : Player{color} {
  if (level <= 1)
    strategy = make_unique<Level1Strategy>();
  else if (level == 2)
    strategy = make_unique<Level2Strategy>();
  else if (level == 3)
    strategy = make_unique<Level3Strategy>();
  else
    strategy = make_unique<Level4Strategy>();
}

bool ComputerPlayer::isHuman() const { return false; }

Move ComputerPlayer::getMove(Board &board) {
  return strategy->generateMove(board, getColor());
}
