#include "Ship.h"

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

bool is_collision(int size, Position position, Direction direction) {
        if (size < 1 || size > 4) {
            return true;
        }

    int start_row = position.row();
    int start_col = position.col();

    try {
        Position start_pos(start_row, start_col);
        if (direction == Direction::Horizontal) {
            Position pos(start_row, start_col + size - 1);
        }
        else if (direction == Direction::Vertical) {
            Position pos(start_row + size - 1, start_col);
        }
    }
    catch (std::exception&) {
        return true;
    }

    return false;
}