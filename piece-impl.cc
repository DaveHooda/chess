module piece;
import <memory>;
import <cctype>;
import chess_types;
import position;

using namespace std;

Piece::Piece(Color color, bool moved) : color{color}, moved{moved} {}

Piece::~Piece() {}

Color Piece::getColor() const { return color; }

bool Piece::hasMoved() const { return moved; }

void Piece::setMoved(bool value) { moved = value; }

bool Piece::canCapture(Position from, Position to) const { return canMove(from, to); }

King::King(Color color, bool moved) : Piece{color, moved} {}

PieceType King::getType() const { return PieceType::King; }

char King::getSymbol() const { return getColor() == Color::White ? 'K' : 'k'; }

bool King::canMove(Position from, Position to) const {
  int dr = from.getRow() - to.getRow();
  if (dr < 0)
    dr = -dr;
  int dc = from.getCol() - to.getCol();
  if (dc < 0)
    dc = -dc;
  return dr <= 1 && dc <= 1 && (dr != 0 || dc != 0);
}

unique_ptr<Piece> King::clone() const { return make_unique<King>(*this); }

Queen::Queen(Color color, bool moved) : Piece{color, moved} {}

PieceType Queen::getType() const { return PieceType::Queen; }

char Queen::getSymbol() const { return getColor() == Color::White ? 'Q' : 'q'; }

bool Queen::canMove(Position from, Position to) const {
  int dr = from.getRow() - to.getRow();
  if (dr < 0)
    dr = -dr;
  int dc = from.getCol() - to.getCol();
  if (dc < 0)
    dc = -dc;
  return (dr == dc || dr == 0 || dc == 0) && (dr != 0 || dc != 0);
}

unique_ptr<Piece> Queen::clone() const { return make_unique<Queen>(*this); }

Bishop::Bishop(Color color, bool moved) : Piece{color, moved} {}

PieceType Bishop::getType() const { return PieceType::Bishop; }

char Bishop::getSymbol() const { return getColor() == Color::White ? 'B' : 'b'; }

bool Bishop::canMove(Position from, Position to) const {
  int dr = from.getRow() - to.getRow();
  if (dr < 0)
    dr = -dr;
  int dc = from.getCol() - to.getCol();
  if (dc < 0)
    dc = -dc;
  return dr == dc && dr != 0;
}

unique_ptr<Piece> Bishop::clone() const { return make_unique<Bishop>(*this); }

Rook::Rook(Color color, bool moved) : Piece{color, moved} {}

PieceType Rook::getType() const { return PieceType::Rook; }

char Rook::getSymbol() const { return getColor() == Color::White ? 'R' : 'r'; }

bool Rook::canMove(Position from, Position to) const {
  return (from.getRow() == to.getRow() || from.getCol() == to.getCol()) && from != to;
}

unique_ptr<Piece> Rook::clone() const { return make_unique<Rook>(*this); }

Knight::Knight(Color color, bool moved) : Piece{color, moved} {}

PieceType Knight::getType() const { return PieceType::Knight; }

char Knight::getSymbol() const { return getColor() == Color::White ? 'N' : 'n'; }

bool Knight::canMove(Position from, Position to) const {
  int dr = from.getRow() - to.getRow();
  if (dr < 0)
    dr = -dr;
  int dc = from.getCol() - to.getCol();
  if (dc < 0)
    dc = -dc;
  return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
}

unique_ptr<Piece> Knight::clone() const { return make_unique<Knight>(*this); }

Pawn::Pawn(Color color, bool moved) : Piece{color, moved} {}

PieceType Pawn::getType() const { return PieceType::Pawn; }

char Pawn::getSymbol() const { return getColor() == Color::White ? 'P' : 'p'; }

bool Pawn::canMove(Position from, Position to) const {
  int dir = getColor() == Color::White ? -1 : 1;
  int dr = to.getRow() - from.getRow();
  int dc = to.getCol() - from.getCol();
  if (dc != 0)
    return false;
  if (dr == dir)
    return true;
  if (!hasMoved() && dr == 2 * dir)
    return true;
  return false;
}

bool Pawn::canCapture(Position from, Position to) const {
  int dir = getColor() == Color::White ? -1 : 1;
  int dr = to.getRow() - from.getRow();
  int dc = to.getCol() - from.getCol();
  if (dc < 0)
    dc = -dc;
  return dr == dir && dc == 1;
}

unique_ptr<Piece> Pawn::clone() const { return make_unique<Pawn>(*this); }

unique_ptr<Piece> makePiece(PieceType type, Color color, bool moved) {
  switch (type) {
  case PieceType::King:
    return make_unique<King>(color, moved);
  case PieceType::Queen:
    return make_unique<Queen>(color, moved);
  case PieceType::Bishop:
    return make_unique<Bishop>(color, moved);
  case PieceType::Rook:
    return make_unique<Rook>(color, moved);
  case PieceType::Knight:
    return make_unique<Knight>(color, moved);
  case PieceType::Pawn:
    return make_unique<Pawn>(color, moved);
  default:
    return nullptr;
  }
}
