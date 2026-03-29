module level1strategy;
import <cstdlib>;
import strategy;
import board;

Move Level1Strategy::generateMove(Board &board, Color color) {
  auto moves = board.allLegalMoves(color);
  if (moves.empty())
    return Move{};
  int idx = rand() % moves.size();
  return moves[idx];
}
