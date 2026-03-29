module textdisplay;
import <iostream>;
import observer;
import board;

using namespace std;

TextDisplay::TextDisplay(Board *board) : board{board} { board->attach(this); }

void TextDisplay::notify() { cout << *board; }

TextDisplay::~TextDisplay() { board->detach(this); }
