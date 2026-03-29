export module board;

import <vector>;
import <memory>;
import <utility>;
import <string>;
import <iostream>;
import chess_types;
import position;
import move;
import subject;
import piece;

export struct MoveRecord {
  Move move;
  std::unique_ptr<Piece> captured;
  bool movedBefore;
  Position rookFrom;
  Position rookTo;
  bool rookMovedBefore;
  Position capturedPos;
  Move previousLastMove;
  bool previousHasLastMove;

  MoveRecord(Move move = Move{}, std::unique_ptr<Piece> captured = nullptr,
             bool movedBefore = false, Position rookFrom = Position{},
             Position rookTo = Position{}, bool rookMovedBefore = false,
             Position capturedPos = Position{},
             Move previousLastMove = Move{}, bool previousHasLastMove = false);

  MoveRecord(MoveRecord &&other) = default;
  MoveRecord &operator=(MoveRecord &&other) = default;
  MoveRecord(const MoveRecord &) = delete;
  MoveRecord &operator=(const MoveRecord &) = delete;
};

export class Board : public Subject {
  std::vector<std::vector<std::unique_ptr<Piece>>> theBoard;
  Color turn;
  std::vector<MoveRecord> history;
  Move lastMove;
  bool hasLastMove;

  bool inBounds(Position p) const;
  bool pathClear(Position from, Position to) const;
  Position findKing(Color color) const;
  bool pawnAttack(Position from, Position to, Color by) const;
  bool canCastle(Position from, Position to) const;
  bool canEnPassant(Position from, Position to) const;
  Move classifyMove(Position from, Position to, PieceType promotionType) const;

public:
  Board();
  Board(const Board &other);
  Board &operator=(const Board &other);

  Color getTurn() const;
  void setTurn(Color next);

  Piece *getPiece(Position p) const;
  bool isEmpty(Position p) const;

  void clear();

  void placePiece(PieceType type, Color color, Position p, bool moved = false);
  void removePiece(Position p);

  void reset();

  bool squareAttacked(Position p, Color by) const;
  bool inCheck(Color color) const;

  bool validSetup() const;

  bool pseudoLegal(Position from, Position to,
                   PieceType promotionType = PieceType::Queen) const;

  void applyMove(const Move &move);
  
  void undoMove();

  bool legalMove(Position from, Position to,
                 PieceType promotionType = PieceType::Queen);

  std::vector<Move> allLegalMoves(Color color);
  bool hasLegalMove(Color color);

  CheckState getCheckState();

  bool makeMove(Position from, Position to,
                PieceType promotionType = PieceType::Queen);

  char drawSquare(Position p) const;
};

export std::ostream &operator<<(std::ostream &out, const Board &b);
