#pragma once
#include "Position.h"
#include <cctype>

enum Direction { Horizontal, Vertical };
class Ship;

bool is_collision(int, Position, Direction);
void parse(const std::string&, Ship&);

class Ship {
    int _size;
    Position _position;
    Direction _direction;

public:
    Ship(int size, Position position, Direction direction);

    Ship(const Ship& ship) : _size(ship._size), _position(ship._position), _direction(ship._direction) {}

    Ship(int size, char direction, int row, char col);

    Ship(const std::string& str) {
        parse(str, *this);
    }

    inline int size() const noexcept {
        return _size;
    }
    inline int row() const noexcept {
        return _position.row();
    }
    inline int col() const noexcept {
        return _position.col();
    }
    inline Position position() const noexcept {
        return _position;
    }
    inline Direction direction() const noexcept {
        return _direction;
    }

    void size(int size);
    void row(int row);
    void col(int col);
    void col(char col);

    void direction(Direction direction);
    void direction(char direction);


    void position(Position position);
    Ship& operator=(const Ship& ship);

    friend void parse(const std::string&, Ship&);
    friend bool is_collision(int, Position, Direction);

};