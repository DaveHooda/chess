module chessgame;

import <memory>;
import <string>;
import <sstream>;
import <iostream>;
import chess_types;
import board;
import player;
import humanplayer;
import computerplayer;
import textdisplay;
import graphicsdisplay;
import xwindow;
import position;
import move;

using namespace std;

Player *ChessGame::currentPlayer() {
  return board.getTurn() == Color::White ? whitePlayer.get() : blackPlayer.get();
}

int ChessGame::parseLevel(const string &name) {
  if (name.length() < 9)
    return 1;
  int level = 1;
  istringstream iss{name.substr(8)};
  if (iss >> level)
    return level;
  return 1;
}

void ChessGame::resetDisplays() {
  textDisplay.reset();
  graphicsDisplay.reset();
  textDisplay = make_unique<TextDisplay>(&board);
  graphicsDisplay = make_unique<GraphicsDisplay>(&board, xwin.get());
}

void ChessGame::setPlayers(const string &white, const string &black) {
  if (white == "human")
    whitePlayer = make_unique<HumanPlayer>(Color::White);
  else
    whitePlayer = make_unique<ComputerPlayer>(Color::White, parseLevel(white));

  if (black == "human")
    blackPlayer = make_unique<HumanPlayer>(Color::Black);
  else
    blackPlayer = make_unique<ComputerPlayer>(Color::Black, parseLevel(black));
}

ChessGame::ChessGame()
    : board{}, whitePlayer{}, blackPlayer{}, whiteScore{0}, blackScore{0},
      active{false} {
  xwin = make_unique<Xwindow>(500, 500);
  textDisplay = make_unique<TextDisplay>(&board);
  graphicsDisplay = make_unique<GraphicsDisplay>(&board, xwin.get());
  board.notifyObservers();
}

Board &ChessGame::getBoard() { return board; }

const Board &ChessGame::getBoard() const { return board; }

bool ChessGame::inGame() const { return active; }

void ChessGame::startGame(const string &white, const string &black) {
  resetDisplays();
  board.reset();
  active = true;
  setPlayers(white, black);
}

void ChessGame::startSetupGame() {
  resetDisplays();
  active = true;
  setPlayers("human", "human");
  board.notifyObservers();
  updateResult();
}

bool ChessGame::currentIsHuman() {
  return currentPlayer() && currentPlayer()->isHuman();
}

bool ChessGame::playHumanMove(Position from, Position to, PieceType promo) {
  if (!active)
    return false;
  bool ok = board.makeMove(from, to, promo);
  if (ok)
    updateResult();
  return ok;
}

bool ChessGame::playComputerMove() {
  if (!active || currentIsHuman())
    return false;
  Move m = currentPlayer()->getMove(board);
  bool ok = board.makeMove(m.from, m.to, m.promotionType);
  if (ok)
    updateResult();
  return ok;
}

void ChessGame::resign() {
  if (!active)
    return;
  if (board.getTurn() == Color::White) {
    ++blackScore;
    cout << "Black wins!" << endl;
  } else {
    ++whiteScore;
    cout << "White wins!" << endl;
  }
  active = false;
}

void ChessGame::updateResult() {
  CheckState state = board.getCheckState();
  if (state == CheckState::WhiteInCheck)
    cout << "White is in check." << endl;
  if (state == CheckState::BlackInCheck)
    cout << "Black is in check." << endl;
  if (state == CheckState::Checkmate) {
    Color winner = oppositeColor(board.getTurn());
    cout << "Checkmate! " << playerColorToString(winner) << " wins!" << endl;
    if (winner == Color::White)
      ++whiteScore;
    else
      ++blackScore;
    active = false;
  }
  if (state == CheckState::Stalemate) {
    cout << "Stalemate!" << endl;
    whiteScore += 0.5;
    blackScore += 0.5;
    active = false;
  }
}

void ChessGame::printFinalScore() const {
  cout << "Final Score:" << endl;
  cout << "White: " << whiteScore << endl;
  cout << "Black: " << blackScore << endl;
}
