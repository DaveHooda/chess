module level4strategy;
import <limits>;
import <vector>;
import chess_types;
import position;
import strategy;
import board;
import move;
import chess_types;

using namespace std;

int Level4Strategy::pieceValue(PieceType type) {
  switch (type) {
  case PieceType::Queen:
    return 9;
  case PieceType::Rook:
    return 5;
  case PieceType::Bishop:
    return 3;
  case PieceType::Knight:
    return 3;
  case PieceType::Pawn:
    return 1;
  default:
    return 0;
  }
}

int Level4Strategy::evaluate(Board &board, Color color) {
  int score = 0;
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      auto *piece = board.getPiece(Position{r, c});
      if (!piece)
        continue;
      int value = pieceValue(piece->getType());
      if (piece->getColor() == color)
        score += value;
      else
        score -= value;
      if ((r == 3 || r == 4) && (c == 3 || c == 4)) {
        if (piece->getColor() == color)
          score += 1;
        else
          score -= 1;
      }
    }
  }
  if (board.inCheck(oppositeColor(color)))
    score += 2;
  if (board.inCheck(color))
    score -= 2;
  if (!board.hasLegalMove(oppositeColor(color)) &&
      board.inCheck(oppositeColor(color)))
    score += 1000;
  if (!board.hasLegalMove(color) && board.inCheck(color))
    score -= 1000;
  return score;
}

Move Level4Strategy::generateMove(Board &board, Color color) {
  auto moves = board.allLegalMoves(color);
  if (moves.empty())
    return Move{};
  int bestScore = numeric_limits<int>::min();
  Move best = moves[0];
  for (auto &m : moves) {
    Board tmp{board};
    tmp.setTurn(color);
    tmp.makeMove(m.from, m.to, m.promotionType);
    int score = evaluate(tmp, color);
    if (score > bestScore) {
      bestScore = score;
      best = m;
    }
  }
  return best;
}
