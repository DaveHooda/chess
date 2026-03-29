module move;
import chess_types;
import position;

Move::Move(Position from, Position to, MoveType type, PieceType promotionType)
    : from{from}, to{to}, type{type}, promotionType{promotionType} {}
