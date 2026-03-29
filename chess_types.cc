export module chess_types;

import <string>;

export enum class Color { White, Black, NoColor };

export enum class PieceType {
  King,
  Queen,
  Bishop,
  Rook,
  Knight,
  Pawn,
  Empty
};

export enum class MoveType {
  Normal,
  Capture,
  Castling,
  EnPassant,
  Promotion,
  PawnTwoStep
};

export enum class GameResult { Ongoing, WhiteWins, BlackWins, Draw };

export enum class CheckState {
  None,
  WhiteInCheck,
  BlackInCheck,
  Checkmate,
  Stalemate
};

export Color oppositeColor(Color c);
export std::string colorToString(Color c);
export std::string playerColorToString(Color c);
export PieceType charToPieceType(char c);

