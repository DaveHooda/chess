module level3strategy;
import <cstdlib>;
import <vector>;
import chess_types;
import strategy;
import board;

using namespace std;

Move Level3Strategy::generateMove(Board &board, Color color) {
  auto moves = board.allLegalMoves(color);
  vector<Move> safest;
  vector<Move> forcing;
  for (auto &m : moves) {
    Board tmp{board};
    tmp.setTurn(color);
    tmp.makeMove(m.from, m.to, m.promotionType);
    bool attacked = tmp.squareAttacked(m.to, oppositeColor(color));
    if (!attacked)
      safest.emplace_back(m);
    if (m.type == MoveType::Capture || m.type == MoveType::EnPassant ||
        tmp.inCheck(oppositeColor(color))) {
      forcing.emplace_back(m);
    }
  }
  if (!safest.empty() && !forcing.empty()) {
    vector<Move> both;
    for (auto &m : safest) {
      for (auto &n : forcing) {
        if (m.from == n.from && m.to == n.to &&
            m.promotionType == n.promotionType) {
          both.emplace_back(m);
        }
      }
    }
    if (!both.empty())
      return both[rand() % both.size()];
  }
  if (!safest.empty())
    return safest[rand() % safest.size()];
  if (!forcing.empty())
    return forcing[rand() % forcing.size()];
  if (moves.empty())
    return Move{};
  return moves[rand() % moves.size()];
}
