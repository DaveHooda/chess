module chess_types;
import <string>;

using namespace std;

Color oppositeColor(Color c) {
  if (c == Color::White)
    return Color::Black;
  if (c == Color::Black)
    return Color::White;
  return Color::NoColor;
}

string colorToString(Color c) {
  if (c == Color::White)
    return "white";
  if (c == Color::Black)
    return "black";
  return "none";
}

string playerColorToString(Color c) {
  if (c == Color::White)
    return "White";
  if (c == Color::Black)
    return "Black";
  return "None";
}

PieceType charToPieceType(char c) {
  switch (c) {
  case 'K':
  case 'k':
    return PieceType::King;
  case 'Q':
  case 'q':
    return PieceType::Queen;
  case 'B':
  case 'b':
    return PieceType::Bishop;
  case 'R':
  case 'r':
    return PieceType::Rook;
  case 'N':
  case 'n':
    return PieceType::Knight;
  case 'P':
  case 'p':
    return PieceType::Pawn;
  default:
    return PieceType::Empty;
  }
}
