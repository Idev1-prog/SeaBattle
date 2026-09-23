#pragma once
#include "Position.h"

enum Direction { Horizontal, Vertical };
class Ship;

bool is_collision(int, Position, Direction);
void parse(const std::string&, Ship&);

class Ship {
    int _size;
    Position _position;
    Direction _direction;

public:
    Ship(int size, Position position, Direction direction) : _size(size), _position(position), _direction(direction) {
        if (is_collision(size, position, direction)) {
            throw std::logic_error("Invalid input: incorrect ship");
        }
    }

    Ship(const Ship& ship) : _size(ship._size), _position(ship._position), _direction(ship._direction) {}

    Ship(int size, char direction, int row, char col) {
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

    void size(int size) {
        if (is_collision(size, _position, _direction)) {
            throw std::logic_error("Invalid input: incorrect ship");
        }
        _size = size;
    }
    void row(int row) {
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
    void col(int col) {
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
    void col(char col) {
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

    void direction(Direction direction) {
        if (is_collision(_size, _position, direction)) {
            throw std::logic_error("Invalid input: incorrect ship");
        }
        _direction = direction;
    }
    void direction(char direction) {
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


    void position(Position position) {
        if (is_collision(_size, position, _direction)) {
            throw std::logic_error("Invalid input: incorrect ship");
        }
        _position = position;
    }
    Ship& operator=(const Ship& ship) {
        _size = ship._size;
        _position = ship._position;
        _direction = ship._direction;
        return *this;
    }

    friend void parse(const std::string&, Ship&);
    friend bool is_collision(int, Position, Direction);

};