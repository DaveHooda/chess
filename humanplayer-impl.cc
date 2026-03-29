module humanplayer;
import player;
import move;
import chess_types;

HumanPlayer::HumanPlayer(Color color) : Player{color}, nextMove{} {}

bool HumanPlayer::isHuman() const { return true; }

void HumanPlayer::setMove(const Move &move) { nextMove = move; }

Move HumanPlayer::getMove(Board &) { return nextMove; }
