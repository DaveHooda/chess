export module graphicsdisplay;

import observer;
import board;
import xwindow;

export class GraphicsDisplay : public Observer {
  static constexpr int cell = 50;
  static constexpr int originX = 50;
  static constexpr int originY = 50;
  Board *board;
  Xwindow *win;

public:
  GraphicsDisplay(Board *board, Xwindow *win);
  void notify() override;
  ~GraphicsDisplay();
};
