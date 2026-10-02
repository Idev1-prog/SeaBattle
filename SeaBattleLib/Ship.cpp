#include "Ship.h"

Ship::Ship(int size, Position position, Direction direction) : _size(size), _position(position), _direction(direction) {
    if (is_collision(size, position, direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}

Ship::Ship(int size, char direction, int row, char col) {
    try {
        _position = Position(row, col);
    }
    catch (std::exception&) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    Direction dir;
    char d = std::toupper(static_cast<unsigned char>(direction));
    if (d == 'H') dir = Direction::Horizontal;
    else if (d == 'V') dir = Direction::Vertical;
    else throw std::logic_error("Invalid input: incorrect ship");

    if (is_collision(size, _position, dir)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _size = size;
    _direction = dir;
}

void Ship::size(int size) {
    if (is_collision(size, _position, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _size = size;
}
void Ship::row(int row) {
    try {
        Position new_pos(row, _position.col());
        if (is_collision(_size, new_pos, _direction)) {
            throw std::logic_error("Invalid input: incorrect ship");
        }
        _position = new_pos;
    }
    catch (std::exception&) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}
void Ship::col(int col) {
    if (col >= 'A' && col <= 'Z') {
        col = col - 'A' + 1;
    }
    else if (col >= 'a' && col <= 'z') {
        col = col - 'a' + 1;
    }
    try {
        Position new_pos(_position.row(), col);
        if (is_collision(_size, new_pos, _direction)) {
            throw std::logic_error("Invalid input: incorrect ship");
        }
        _position = new_pos;
    }
    catch (std::exception&) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}
void Ship::col(char col) {
    try {
        Position new_pos(_position.row(), col);
        if (is_collision(_size, new_pos, _direction)) {
            throw std::logic_error("Invalid input: incorrect ship");
        }
        _position = new_pos;
    }
    catch (std::exception&) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}

void Ship::direction(Direction direction) {
    if (is_collision(_size, _position, direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _direction = direction;
}
void Ship::direction(char direction) {
    Direction dir;
    char d = std::toupper(static_cast<unsigned char>(direction));
    if (d == 'H') dir = Direction::Horizontal;
    else if (d == 'V') dir = Direction::Vertical;
    else throw std::logic_error("Invalid input: incorrect ship");

    if (is_collision(_size, _position, dir)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _direction = dir;
}


void Ship::position(Position position) {
    if (is_collision(_size, position, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _position = position;
}
Ship& Ship::operator=(const Ship& ship) {
    _size = ship._size;
    _position = ship._position;
    _direction = ship._direction;
    return *this;
}

void parse(const std::string& str, Ship& ship) {
    int row = 0;
    char col = 0;
    char dir = 0;
    Direction direction;
    size_t i = 0;
    int size = 0;
    while (i < str.size() && std::isspace(str[i])) {
        i++;
    }
    while (i < str.size() && std::isdigit(str[i])) {
        size = size * 10 + (str[i] - '0');
        i++;
    }
    if (size == 0) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    while (i < str.size() && std::isspace(str[i])) {
        i++;
    }
    if (i < str.size() && std::isalpha(str[i])) {
        dir = str[i];
        if (dir == 'V' || dir == 'v') {
            direction = Direction::Vertical;
        }
        else if (dir == 'H' || dir == 'h') {
            direction = Direction::Horizontal;
        }
        else {
            throw std::logic_error("Invalid input: incorrect ship");
        }
        i++;
        if (str[i] != ' ') {
            throw std::logic_error("Invalid input: incorrect ship");
        }
    }
    else {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    while (i < str.size() && std::isspace(str[i])) {
        i++;
    }
    while (i < str.size() && std::isdigit(str[i])) {
        row = row * 10 + (str[i] - '0');
        i++;
    }
    while (i < str.size() && std::isspace(str[i])) {
        i++;
    }
    if (i < str.size() && std::isalpha(str[i])) {
        col = std::toupper(str[i]);
        i++;
    }
    while (i < str.size() && std::isspace(str[i])) {
        i++;
    }
    if (i != str.size()) {
        throw std::logic_error("Invalid input: incorrect ship");
    }

    try {
        ship = Ship(size, Position(row, col), Direction(direction));
    }
    catch (std::exception&) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}

bool is_collision(int size, Position position, Direction direction) { // эксперементальное исправление
    if (size < 1 || size > 4) {
        return true;
    }

    const int end_row = (direction == Direction::Vertical)
        ? position.row() + size - 1
        : position.row();
    const int end_col = (direction == Direction::Horizontal)
        ? position.col() + size - 1
        : position.col();

    if (end_row > Position::max_row()) return true;
    if (end_col > Position::max_col()) return true;

    return false;
}