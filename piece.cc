export module piece;

import <memory>;
import chess_types;
import position;

export class Piece {
  Color color;
  bool moved;

public:
  Piece(Color color, bool moved = false);
  virtual ~Piece();

  Color getColor() const;
  bool hasMoved() const;
  void setMoved(bool value);

  virtual PieceType getType() const = 0;
  virtual char getSymbol() const = 0;
  virtual bool canMove(Position from, Position to) const = 0;
  virtual bool canCapture(Position from, Position to) const;
  virtual std::unique_ptr<Piece> clone() const = 0;
};

export class King : public Piece {
public:
  King(Color color, bool moved = false);
  PieceType getType() const override;
  char getSymbol() const override;
  bool canMove(Position from, Position to) const override;
  std::unique_ptr<Piece> clone() const override;
};

export class Queen : public Piece {
public:
  Queen(Color color, bool moved = false);
  PieceType getType() const override;
  char getSymbol() const override;
  bool canMove(Position from, Position to) const override;
  std::unique_ptr<Piece> clone() const override;
};

export class Bishop : public Piece {
public:
  Bishop(Color color, bool moved = false);
  PieceType getType() const override;
  char getSymbol() const override;
  bool canMove(Position from, Position to) const override;
  std::unique_ptr<Piece> clone() const override;
};

export class Rook : public Piece {
public:
  Rook(Color color, bool moved = false);
  PieceType getType() const override;
  char getSymbol() const override;
  bool canMove(Position from, Position to) const override;
  std::unique_ptr<Piece> clone() const override;
};

export class Knight : public Piece {
public:
  Knight(Color color, bool moved = false);
  PieceType getType() const override;
  char getSymbol() const override;
  bool canMove(Position from, Position to) const override;
  std::unique_ptr<Piece> clone() const override;
};

export class Pawn : public Piece {
public:
  Pawn(Color color, bool moved = false);
  PieceType getType() const override;
  char getSymbol() const override;
  bool canMove(Position from, Position to) const override;
  bool canCapture(Position from, Position to) const override;
  std::unique_ptr<Piece> clone() const override;
};

export std::unique_ptr<Piece> makePiece(PieceType type, Color color, bool moved = false);
