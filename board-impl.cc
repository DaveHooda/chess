module board;

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

using namespace std;

MoveRecord::MoveRecord(Move move, unique_ptr<Piece> captured, bool movedBefore,
                       Position rookFrom, Position rookTo, bool rookMovedBefore,
                       Position capturedPos, Move previousLastMove,
                       bool previousHasLastMove)
    : move{move}, captured{std::move(captured)}, movedBefore{movedBefore},
      rookFrom{rookFrom}, rookTo{rookTo}, rookMovedBefore{rookMovedBefore},
      capturedPos{capturedPos}, previousLastMove{previousLastMove},
      previousHasLastMove{previousHasLastMove} {}

bool Board::inBounds(Position p) const { return p.isValid(); }

bool Board::pathClear(Position from, Position to) const {
  int dr = 0;
  int dc = 0;
  if (to.getRow() > from.getRow())
    dr = 1;
  if (to.getRow() < from.getRow())
    dr = -1;
  if (to.getCol() > from.getCol())
    dc = 1;
  if (to.getCol() < from.getCol())
    dc = -1;
  int r = from.getRow() + dr;
  int c = from.getCol() + dc;
  while (r != to.getRow() || c != to.getCol()) {
    if (theBoard[r][c])
      return false;
    r += dr;
    c += dc;
  }
  return true;
}

Position Board::findKing(Color color) const {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      if (theBoard[r][c] && theBoard[r][c]->getType() == PieceType::King &&
          theBoard[r][c]->getColor() == color) {
        return Position{r, c};
      }
    }
  }
  return Position{};
}

bool Board::pawnAttack(Position from, Position to, Color by) const {
  int dir = by == Color::White ? -1 : 1;
  int dr = to.getRow() - from.getRow();
  int dc = to.getCol() - from.getCol();
  if (dc < 0)
    dc = -dc;
  return dr == dir && dc == 1;
}

bool Board::canCastle(Position from, Position to) const {
  if (!getPiece(from) || getPiece(from)->getType() != PieceType::King)
    return false;
  if (getPiece(from)->hasMoved())
    return false;
  int row = from.getRow();
  int dc = to.getCol() - from.getCol();
  if (row != to.getRow())
    return false;
  if (dc != 2 && dc != -2)
    return false;
  int rookCol = dc > 0 ? 7 : 0;
  Position rookPos{row, rookCol};
  if (!getPiece(rookPos) || getPiece(rookPos)->getType() != PieceType::Rook)
    return false;
  if (getPiece(rookPos)->hasMoved())
    return false;
  int step = dc > 0 ? 1 : -1;
  for (int c = from.getCol() + step; c != rookCol; c += step) {
    if (theBoard[row][c])
      return false;
  }
  Color color = getPiece(from)->getColor();
  if (squareAttacked(from, oppositeColor(color)))
    return false;
  if (squareAttacked(Position{row, from.getCol() + step}, oppositeColor(color)))
    return false;
  if (squareAttacked(to, oppositeColor(color)))
    return false;
  return true;
}

bool Board::canEnPassant(Position from, Position to) const {
  if (!hasLastMove)
    return false;
  if (lastMove.type != MoveType::PawnTwoStep)
    return false;
  auto *piece = getPiece(from);
  if (!piece || piece->getType() != PieceType::Pawn)
    return false;
  int dir = piece->getColor() == Color::White ? -1 : 1;
  if (to.getRow() - from.getRow() != dir)
    return false;
  int dc = to.getCol() - from.getCol();
  if (dc < 0)
    dc = -dc;
  if (dc != 1)
    return false;
  Position enemyPawn = lastMove.to;
  if (enemyPawn.getRow() != from.getRow())
    return false;
  if (enemyPawn.getCol() != to.getCol())
    return false;
  return true;
}

Move Board::classifyMove(Position from, Position to,
                         PieceType promotionType) const {
  auto *piece = getPiece(from);
  if (!piece)
    return Move{};
  MoveType type = MoveType::Normal;
  if (piece->getType() == PieceType::King) {
    int dc = to.getCol() - from.getCol();
    if (dc < 0)
      dc = -dc;
    if (dc == 2)
      type = MoveType::Castling;
  }
  if (piece->getType() == PieceType::Pawn) {
    int dr = to.getRow() - from.getRow();
    if (dr < 0)
      dr = -dr;
    if (dr == 2)
      type = MoveType::PawnTwoStep;
    if (to.getCol() != from.getCol() && !getPiece(to))
      type = MoveType::EnPassant;
    if ((piece->getColor() == Color::White && to.getRow() == 0) ||
        (piece->getColor() == Color::Black && to.getRow() == 7)) {
      type = MoveType::Promotion;
    }
  }
  if (getPiece(to))
    type = MoveType::Capture;
  if (piece->getType() == PieceType::Pawn &&
      ((piece->getColor() == Color::White && to.getRow() == 0) ||
       (piece->getColor() == Color::Black && to.getRow() == 7))) {
    type = MoveType::Promotion;
  }
  return Move{from, to, type, promotionType};
}

Board::Board() : theBoard{8}, turn{Color::White}, lastMove{}, hasLastMove{false} {
  for (int i = 0; i < 8; ++i)
    theBoard[i] = vector<unique_ptr<Piece>>(8);
  reset();
}

Board::Board(const Board &other)
    : theBoard{8}, turn{other.turn}, lastMove{other.lastMove},
      hasLastMove{other.hasLastMove} {
  for (int i = 0; i < 8; ++i) {
    theBoard[i] = vector<unique_ptr<Piece>>(8);
    for (int j = 0; j < 8; ++j) {
      if (other.theBoard[i][j])
        theBoard[i][j] = other.theBoard[i][j]->clone();
    }
  }
}

Board &Board::operator=(const Board &other) {
  if (this == &other)
    return *this;
  Board temp{other};
  swap(theBoard, temp.theBoard);
  swap(turn, temp.turn);
  swap(lastMove, temp.lastMove);
  swap(hasLastMove, temp.hasLastMove);
  history.clear();
  return *this;
}

Color Board::getTurn() const { return turn; }

void Board::setTurn(Color next) {
  turn = next;
  notifyObservers();
}

Piece *Board::getPiece(Position p) const {
  if (!inBounds(p))
    return nullptr;
  return theBoard[p.getRow()][p.getCol()].get();
}

bool Board::isEmpty(Position p) const { return getPiece(p) == nullptr; }

void Board::clear() {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      theBoard[r][c].reset();
    }
  }
  history.clear();
  turn = Color::White;
  lastMove = Move{};
  hasLastMove = false;
  notifyObservers();
}

void Board::placePiece(PieceType type, Color color, Position p, bool moved) {
  if (!p.isValid())
    return;
  theBoard[p.getRow()][p.getCol()] = makePiece(type, color, moved);
  notifyObservers();
}

void Board::removePiece(Position p) {
  if (!p.isValid())
    return;
  theBoard[p.getRow()][p.getCol()].reset();
  notifyObservers();
}

void Board::reset() {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      theBoard[r][c].reset();
    }
  }
  history.clear();
  turn = Color::White;
  lastMove = Move{};
  hasLastMove = false;
  for (int c = 0; c < 8; ++c) {
    theBoard[1][c] = makePiece(PieceType::Pawn, Color::Black, false);
    theBoard[6][c] = makePiece(PieceType::Pawn, Color::White, false);
  }
  theBoard[0][0] = makePiece(PieceType::Rook, Color::Black, false);
  theBoard[0][1] = makePiece(PieceType::Knight, Color::Black, false);
  theBoard[0][2] = makePiece(PieceType::Bishop, Color::Black, false);
  theBoard[0][3] = makePiece(PieceType::Queen, Color::Black, false);
  theBoard[0][4] = makePiece(PieceType::King, Color::Black, false);
  theBoard[0][5] = makePiece(PieceType::Bishop, Color::Black, false);
  theBoard[0][6] = makePiece(PieceType::Knight, Color::Black, false);
  theBoard[0][7] = makePiece(PieceType::Rook, Color::Black, false);

  theBoard[7][0] = makePiece(PieceType::Rook, Color::White, false);
  theBoard[7][1] = makePiece(PieceType::Knight, Color::White, false);
  theBoard[7][2] = makePiece(PieceType::Bishop, Color::White, false);
  theBoard[7][3] = makePiece(PieceType::Queen, Color::White, false);
  theBoard[7][4] = makePiece(PieceType::King, Color::White, false);
  theBoard[7][5] = makePiece(PieceType::Bishop, Color::White, false);
  theBoard[7][6] = makePiece(PieceType::Knight, Color::White, false);
  theBoard[7][7] = makePiece(PieceType::Rook, Color::White, false);
  notifyObservers();
}

bool Board::squareAttacked(Position p, Color by) const {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      auto *piece = getPiece(Position{r, c});
      if (!piece || piece->getColor() != by)
        continue;
      Position from{r, c};
      if (piece->getType() == PieceType::Pawn) {
        if (pawnAttack(from, p, by))
          return true;
      } else if (piece->canCapture(from, p)) {
        if (piece->getType() == PieceType::Knight || piece->getType() == PieceType::King)
          return true;
        if (pathClear(from, p))
          return true;
      }
    }
  }
  return false;
}

bool Board::inCheck(Color color) const {
  Position kingPos = findKing(color);
  return kingPos.isValid() && squareAttacked(kingPos, oppositeColor(color));
}

bool Board::validSetup() const {
  int whiteKings = 0;
  int blackKings = 0;
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      auto *piece = getPiece(Position{r, c});
      if (!piece)
        continue;
      if (piece->getType() == PieceType::King) {
        if (piece->getColor() == Color::White)
          ++whiteKings;
        else
          ++blackKings;
      }
      if (piece->getType() == PieceType::Pawn && (r == 0 || r == 7))
        return false;
    }
  }
  if (whiteKings != 1 || blackKings != 1)
    return false;
  if (inCheck(Color::White) || inCheck(Color::Black))
    return false;
  return true;
}

bool Board::pseudoLegal(Position from, Position to,
                        PieceType promotionType) const {
  auto *piece = getPiece(from);
  if (!piece || !to.isValid())
    return false;
  if (from == to)
    return false;
  auto *dest = getPiece(to);
  if (dest && dest->getColor() == piece->getColor())
    return false;

  if (piece->getType() == PieceType::King && canCastle(from, to))
    return true;
  if (piece->getType() == PieceType::Pawn) {
    if (from.getCol() == to.getCol()) {
      if (!piece->canMove(from, to))
        return false;
      if (dest)
        return false;
      int dr = to.getRow() - from.getRow();
      if (dr < 0)
        dr = -dr;
      if (dr == 2) {
        int mid = (from.getRow() + to.getRow()) / 2;
        if (theBoard[mid][from.getCol()])
          return false;
      }
      return true;
    }
    if (!piece->canCapture(from, to))
      return false;
    if (dest && dest->getColor() != piece->getColor())
      return true;
    return canEnPassant(from, to);
  }

  if (!piece->canMove(from, to))
    return false;
  if (piece->getType() == PieceType::Knight)
    return true;
  return pathClear(from, to);
}

void Board::applyMove(const Move &move) {
  auto *movingPiece = getPiece(move.from);
  bool movedBefore = movingPiece->hasMoved();
  unique_ptr<Piece> captured;
  Position capturedPos = move.to;
  Position rookFrom{};
  Position rookTo{};
  bool rookMovedBefore = false;

  if (move.type == MoveType::EnPassant) {
    int dir = movingPiece->getColor() == Color::White ? 1 : -1;
    capturedPos = Position{move.to.getRow() + dir, move.to.getCol()};
    captured =
        std::move(theBoard[capturedPos.getRow()][capturedPos.getCol()]);
  } else {
    captured = std::move(theBoard[move.to.getRow()][move.to.getCol()]);
  }

  auto piecePtr =
      std::move(theBoard[move.from.getRow()][move.from.getCol()]);
  piecePtr->setMoved(true);
  if (move.type == MoveType::Promotion) {
    Color color = piecePtr->getColor();
    piecePtr = makePiece(move.promotionType, color, true);
  }
  theBoard[move.to.getRow()][move.to.getCol()] = std::move(piecePtr);

  if (move.type == MoveType::Castling) {
    int row = move.from.getRow();
    if (move.to.getCol() > move.from.getCol()) {
      rookFrom = Position{row, 7};
      rookTo = Position{row, 5};
    } else {
      rookFrom = Position{row, 0};
      rookTo = Position{row, 3};
    }
    rookMovedBefore = getPiece(rookFrom)->hasMoved();
    auto rook = std::move(theBoard[rookFrom.getRow()][rookFrom.getCol()]);
    rook->setMoved(true);
    theBoard[rookTo.getRow()][rookTo.getCol()] = std::move(rook);
  }

  MoveRecord rec{move, std::move(captured), movedBefore, rookFrom,
                 rookTo, rookMovedBefore, capturedPos, lastMove,
                 hasLastMove};
  history.emplace_back(MoveRecord{Move{}, nullptr});
  history.back() = std::move(rec);
  lastMove = move;
  hasLastMove = true;
}

void Board::undoMove() {
  if (history.empty())
    return;
  MoveRecord rec = std::move(history.back());
  history.pop_back();

  auto piecePtr =
      std::move(theBoard[rec.move.to.getRow()][rec.move.to.getCol()]);
  Color color = piecePtr->getColor();
  if (rec.move.type == MoveType::Promotion) {
    piecePtr = makePiece(PieceType::Pawn, color, rec.movedBefore);
  } else {
    piecePtr->setMoved(rec.movedBefore);
  }
  theBoard[rec.move.from.getRow()][rec.move.from.getCol()] =
      std::move(piecePtr);

  if (rec.move.type == MoveType::Castling) {
    auto rook = std::move(theBoard[rec.rookTo.getRow()][rec.rookTo.getCol()]);
    rook->setMoved(rec.rookMovedBefore);
    theBoard[rec.rookFrom.getRow()][rec.rookFrom.getCol()] = std::move(rook);
  }

  theBoard[rec.move.to.getRow()][rec.move.to.getCol()].reset();
  if (rec.captured) {
    theBoard[rec.capturedPos.getRow()][rec.capturedPos.getCol()] =
        std::move(rec.captured);
  }

  lastMove = rec.previousLastMove;
  hasLastMove = rec.previousHasLastMove;
}

bool Board::legalMove(Position from, Position to, PieceType promotionType) {
  auto *piece = getPiece(from);
  if (!piece)
    return false;
  if (piece->getColor() != turn)
    return false;
  if (!pseudoLegal(from, to, promotionType))
    return false;
  Move move = classifyMove(from, to, promotionType);
  applyMove(move);
  bool bad = inCheck(turn);
  undoMove();
  return !bad;
}

vector<Move> Board::allLegalMoves(Color color) {
  vector<Move> moves;
  Color oldTurn = turn;
  turn = color;
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      auto *piece = getPiece(Position{r, c});
      if (!piece || piece->getColor() != color)
        continue;
      for (int rr = 0; rr < 8; ++rr) {
        for (int cc = 0; cc < 8; ++cc) {
          Position from{r, c};
          Position to{rr, cc};
          if (piece->getType() == PieceType::Pawn && (rr == 0 || rr == 7)) {
            PieceType promos[4] = {PieceType::Queen, PieceType::Rook,
                                   PieceType::Bishop, PieceType::Knight};
            for (auto promo : promos) {
              if (legalMove(from, to, promo))
                moves.emplace_back(classifyMove(from, to, promo));
            }
          } else if (legalMove(from, to)) {
            moves.emplace_back(classifyMove(from, to, PieceType::Queen));
          }
        }
      }
    }
  }
  turn = oldTurn;
  return moves;
}

bool Board::hasLegalMove(Color color) {
  auto moves = allLegalMoves(color);
  return !moves.empty();
}

CheckState Board::getCheckState() {
  bool white = inCheck(Color::White);
  bool black = inCheck(Color::Black);
  if (!hasLegalMove(turn)) {
    if (inCheck(turn))
      return CheckState::Checkmate;
    return CheckState::Stalemate;
  }
  if (white)
    return CheckState::WhiteInCheck;
  if (black)
    return CheckState::BlackInCheck;
  return CheckState::None;
}

bool Board::makeMove(Position from, Position to, PieceType promotionType) {
  if (!legalMove(from, to, promotionType))
    return false;
  Move move = classifyMove(from, to, promotionType);
  applyMove(move);
  turn = oppositeColor(turn);
  notifyObservers();
  return true;
}

char Board::drawSquare(Position p) const {
  auto *piece = getPiece(p);
  if (piece)
    return piece->getSymbol();
  return ((p.getRow() + p.getCol()) % 2 == 0) ? ' ' : '_';
}

ostream &operator<<(ostream &out, const Board &b) {
  for (int r = 0; r < 8; ++r) {
    out << 8 - r << ' ';
    for (int c = 0; c < 8; ++c)
      out << b.drawSquare(Position{r, c});
    out << endl;
  }
  out << "  abcdefgh" << endl;
  return out;
}
