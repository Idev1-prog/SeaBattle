#pragma once
#include "Position.h"

enum Direction { Horizontal, Vertical };

bool is_collision(int size, Position position, Direction direction);
Direction to_direction(char direction);
int to_col(char col);

class Ship
{
private:
	int _size;
	Position _position;
	Direction _direction;

public:
	Ship(int size, const Position& position, Direction direction);
	Ship(int size, const Position& position) : Ship(size, position, Horizontal) {}
	Ship(int size, char direction, int row, char col);
	Ship(const std::string& str);

	inline int size() const noexcept { return _size; }
	inline int row() const noexcept { return _position.row(); }
	inline int col() const noexcept { return _position.col(); }
	inline Position position() const noexcept { return _position; }
	inline Direction direction() const noexcept { return _direction; }

	void size(int);
	void row(int);
	void col(int);
	void col(char);
	void direction(Direction);
	void direction(char);
	void position(const Position&);

	void rotate();

	friend bool is_collision(int size, Position position, Direction direction);
	friend void parse(const std::string& str, Ship& ship);
};