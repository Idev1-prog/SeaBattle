#include "Ship.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

int to_col(char col) {
    char upper = toupper(col);
    if (upper >= 'A' && upper <= 'J') {
        return upper - 'A' + 1;
    }
    throw std::logic_error("Invalid input: incorrect ship");
}

Direction to_direction(char direction) {
    char upper = toupper(direction);
    if (upper == 'H') return Horizontal;
    if (upper == 'V') return Vertical;
    throw std::logic_error("Invalid input: incorrect ship");
}

bool is_collision(int size, Position position, Direction direction) {
    if (size < 1 || size > 4) return true; // Invalid size

    int max_r = Position::max_row();
    int max_c = Position::max_col();

    int r = position.row();
    int c = position.col();

    if (r < 1 || r > max_r || c < 1 || c > max_c) return true;

    if (direction == Horizontal) {
        if (c + size - 1 > max_c) return true;
    }
    else {
        if (r + size - 1 > max_r) return true;
    }
    return false;
}

Ship::Ship(int size, const Position& position, Direction direction)
    : _size(size), _position(position), _direction(direction)
{
    if (is_collision(_size, _position, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}

Ship::Ship(int size, char direction, int row, char col)
    : Ship(size, Position(row, col), to_direction(direction))
{
}

Ship::Ship(const std::string& str) {
    parse(str, *this);
}

void Ship::size(int s) {
    if (is_collision(s, _position, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _size = s;
}

void Ship::row(int r) {
    Position newPos(r, _position.col());
    if (is_collision(_size, newPos, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _position = newPos;
}

void Ship::col(int c) {
    Position newPos(_position.row(), c);
    if (is_collision(_size, newPos, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _position = newPos;
}

void Ship::col(char c) {
    int col_val = to_col(c);
    Position newPos(_position.row(), col_val);
    if (is_collision(_size, newPos, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _position = newPos;
}

void Ship::direction(Direction d) {
    if (is_collision(_size, _position, d)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _direction = d;
}

void Ship::direction(char d) {
    Direction dir = to_direction(d);
    if (is_collision(_size, _position, dir)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _direction = dir;
}

void Ship::position(const Position& p) {
    if (is_collision(_size, p, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _position = p;
}

void Ship::rotate() {
    Direction newDir = (_direction == Horizontal) ? Vertical : Horizontal;
    if (is_collision(_size, _position, newDir)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _direction = newDir;
}

void parse(const std::string& str, Ship& ship) {
    std::istringstream iss(str);
    int size;
    char dirChar;
    std::string posToken;

    if (!(iss >> size >> dirChar >> posToken)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }

    Direction dir = to_direction(dirChar);

    size_t i = 0;
    while (i < posToken.length() && std::isdigit(posToken[i])) {
        i++;
    }

    if (i == 0 || i == posToken.length()) {
        throw std::logic_error("Invalid input: incorrect ship");
    }

    std::string rowStr = posToken.substr(0, i);
    char colChar = posToken[i];

    int row = std::stoi(rowStr);
    int col = to_col(colChar);

    Position pos(row, col);

    ship = Ship(size, pos, dir);
}