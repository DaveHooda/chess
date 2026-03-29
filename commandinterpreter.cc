export module commandinterpreter;

import <iostream>;
import <string>;
import chessgame;
import chess_types;
import position;

export class CommandInterpreter {
  ChessGame game;

  Color parseColor(const std::string &s);
  PieceType parsePromotion(const std::string &s);

public:
  void run();
};
