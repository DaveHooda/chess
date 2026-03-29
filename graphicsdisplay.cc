export module graphicsdisplay;

import observer;
import board;
import xwindow;

export class GraphicsDisplay : public Observer {
  Board *board;
  Xwindow *win;

public:
  GraphicsDisplay(Board *board, Xwindow *win);
  void notify() override;
  ~GraphicsDisplay();
};
