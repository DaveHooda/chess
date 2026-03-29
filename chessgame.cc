export module chessgame;

import <memory>;
import <string>;
import <iostream>;
import chess_types;
import position;
import board;
import player;
import humanplayer;
import computerplayer;
import textdisplay;
import graphicsdisplay;
import xwindow;

export class ChessGame {
  Board board;
  std::unique_ptr<Xwindow> xwin;
  std::unique_ptr<TextDisplay> textDisplay;
  std::unique_ptr<GraphicsDisplay> graphicsDisplay;
  std::unique_ptr<Player> whitePlayer;
  std::unique_ptr<Player> blackPlayer;
  double whiteScore;
  double blackScore;
  bool active;

  Player *currentPlayer();
  int parseLevel(const std::string &name);
  void resetDisplays();
  void setPlayers(const std::string &white, const std::string &black);

public:
  ChessGame();

  Board &getBoard();
  const Board &getBoard() const;
  bool inGame() const;

  void startGame(const std::string &white, const std::string &black);
  void startSetupGame();

  bool currentIsHuman();

  bool playHumanMove(Position from, Position to,
                     PieceType promo = PieceType::Queen);
  bool playComputerMove();

  void resign();
  void updateResult();
  void printFinalScore() const;
};
