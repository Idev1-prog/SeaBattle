#pragma once
#include <iostream>
#include <regex>
#include <iomanip>
#include <sstream>
class Position
{
private:
	int _row;        // строка
	int _col;        // столбец
	static const int _max_row = 10;
	static const int _max_col = 10;
public:
	Position();
	Position(int, int);
	Position(const std::string& str) {
		int row, col;
		row = 1;
		col = 1;
		std::istringstream iss(str);
		char ch1, ch2, ch3;

		iss >> ch1 >> row >> ch2 >> col >> ch3;

		if (ch1 != '(' || ch2 != ',' || ch3 != ')' || iss.fail()) {
			throw std::logic_error("Invalid input: incorrect position");
		}

		if (row < 1 || row > _max_row || col < 1 || col > _max_col) {
			throw std::logic_error("Invalid input: incorrect position");
		}

		_row = row;
		_col = col;
	}

	Position(const Position& other) : _row(other._row), _col(other._col) {}
	~Position() = default;

	void row(int);
	void col(int);
	static int max_row() noexcept { return _max_row; }
	static int max_col() noexcept { return _max_col; }
	inline int row() const noexcept { return _row; }
	inline int col() const noexcept { return _col; }

	friend Position parse(const std::string& str);
	friend std::string to_string(const Position& pos);

};