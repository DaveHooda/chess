module level2strategy;
import <cstdlib>;
import <vector>;
import chess_types;
import strategy;
import board;

using namespace std;

Move Level2Strategy::generateMove(Board &board, Color color) {
  auto moves = board.allLegalMoves(color);
  vector<Move> best;
  for (auto &m : moves) {
    Board tmp{board};
    tmp.setTurn(color);
    tmp.makeMove(m.from, m.to, m.promotionType);
    if (m.type == MoveType::Capture || m.type == MoveType::EnPassant ||
        tmp.inCheck(oppositeColor(color))) {
      best.emplace_back(m);
    }
  }
  if (best.empty())
    best = moves;
  if (best.empty())
    return Move{};
  return best[rand() % best.size()];
}
