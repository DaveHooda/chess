module graphicsdisplay;
import observer;
import board;
import xwindow;
import position;

using namespace std;

GraphicsDisplay::GraphicsDisplay(Board *board, Xwindow *win)
    : board{board}, win{win} {
  board->attach(this);
}

void GraphicsDisplay::notify() {
  const int n = 8;
  const int dim = n * cell;
  win->fillRectangle(0, 0, 500, 500, Xwindow::White);
  for (int r = 0; r < n; ++r) {
    for (int c = 0; c < n; ++c) {
      int x = originX + c * cell;
      int y = originY + r * cell;
      bool light = ((r + c) % 2 == 0);
      win->fillRectangle(x, y, cell, cell,
                           light ? Xwindow::White : Xwindow::Brown);
      char sym = board->drawSquare(Position{r, c});
      if (sym != ' ' && sym != '_') {
        string s(1, sym);
        win->drawString(x + 18, y + 32, s);
      }
    }
  }
  for (int c = 0; c < n; ++c) {
    string s(1, static_cast<char>('a' + c));
    win->drawString(originX + c * cell + 18, originY + dim + 20, s);
  }
  for (int r = 0; r < n; ++r) {
    string s(1, static_cast<char>('8' - r));
    win->drawString(originX - 25, originY + r * cell + 32, s);
  }
}

GraphicsDisplay::~GraphicsDisplay() { board->detach(this); }
