export module move;

import chess_types;
import position;

export class Move {
public:
  Position from;
  Position to;
  MoveType type;
  PieceType promotionType;

  Move(Position from = Position{}, Position to = Position{},
       MoveType type = MoveType::Normal,
       PieceType promotionType = PieceType::Queen);
};
