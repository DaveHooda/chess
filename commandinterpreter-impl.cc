module commandinterpreter;
import <iostream>;
import <string>;
import chessgame;
import chess_types;
import position;

using namespace std;

Color CommandInterpreter::parseColor(const string &s) {
  if (s == "white")
    return Color::White;
  if (s == "black")
    return Color::Black;
  return Color::NoColor;
}

PieceType CommandInterpreter::parsePromotion(const string &s) {
  if (s.empty())
    return PieceType::Queen;
  return charToPieceType(s[0]);
}

void CommandInterpreter::run() {
  string cmd;
  while (cin >> cmd) {
    if (cmd == "game") {
      string w, b;
      cin >> w >> b;
      game.startGame(w, b);
    } else if (cmd == "resign") {
      game.resign();
    } else if (cmd == "move") {
      if (!game.inGame())
        continue;
      if (!game.currentIsHuman()) {
        game.playComputerMove();
      } else {
        string from, to, promo;
        cin >> from >> to;
        if (!(cin.peek() == '\n' || cin.peek() == EOF))
          cin >> promo;
        PieceType promotion = parsePromotion(promo);
        if (!promo.empty() && promotion == PieceType::Empty) {
          cout << "Invalid move." << endl;
          continue;
        }
        if (!game.playHumanMove(Position::fromChess(from), Position::fromChess(to),
                                promotion)) {
          cout << "Invalid move." << endl;
        }
      }
    } else if (cmd == "setup") {
      if (game.inGame())
        continue;
      auto &board = game.getBoard();
      board.clear();
      string op;
      bool startConfiguredGame = false;
      while (cin >> op) {
        if (op == "+") {
          string piece, pos;
          cin >> piece >> pos;
          char ch = piece[0];
          Color color =
              ('A' <= ch && ch <= 'Z') ? Color::White : Color::Black;
          board.placePiece(charToPieceType(ch), color, Position::fromChess(pos));
        } else if (op == "-") {
          string pos;
          cin >> pos;
          board.removePiece(Position::fromChess(pos));
        } else if (op == "=") {
          string color;
          cin >> color;
          board.setTurn(parseColor(color));
        } else if (op == "done") {
          if (board.validSetup()) {
            startConfiguredGame = true;
            break;
          } else {
            cout << "Invalid setup." << endl;
          }
        }
      }
      if (startConfiguredGame)
        game.startSetupGame();
    }
  }
  game.printFinalScore();
}
