module player;
import chess_types;
import move;
import board;

Player::Player(Color color) : color{color} {}

Player::~Player() {}

Color Player::getColor() const { return color; }
