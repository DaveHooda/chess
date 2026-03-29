export module textdisplay;

import <iostream>;
import observer;
import board;

export class TextDisplay : public Observer {
  Board *board;

public:
  TextDisplay(Board *board);
  void notify() override;
  ~TextDisplay();
};
