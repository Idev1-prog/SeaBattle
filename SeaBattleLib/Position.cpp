#include "Position.h"

Position::Position() :
	_row(1),
	_col(1) {
}

Position::Position(int row, int col) {
	if (row < 1 || row > _max_row)
		throw std::logic_error("Invalid input: incorrect position");
	if (col < 1 || col > _max_col)
		throw std::logic_error("Invalid input: incorrect position");

	_row = row;
	_col = col;
}

void Position::row(int row) {
	if (row < 1 || row > _max_row)
		throw std::logic_error("Invalid input: incorrect position");
	_row = row;
}

void Position::col(int col) {
	if (col < 1 || col > _max_col)
		throw std::logic_error("Invalid input: incorrect position");

	_col = col;
}

Position parse(const std::string& str) {
	std::istringstream iss(str);
	char ch1, ch2, ch3;
	int row, col;

	iss >> ch1 >> row >> ch2 >> col >> ch3;

	if (ch1 != '(' || ch2 != ',' || ch3 != ')' || iss.fail()) {
		throw std::logic_error("Invalid input: incorrect position");
	}

	if (row < 1 || row > Position::_max_row || col < 1 || col > Position::_max_col) {
		throw std::logic_error("Invalid input: incorrect position");
	}

	return Position(row, col);
}

std::string to_string(const Position& pos) {
	std::string str;
	str.append(1, '(');
	str.append(std::to_string(pos._row));
	str.append(", ");
	str.append(std::to_string(pos._col));
	str.append(1, ')');

	return str;
}