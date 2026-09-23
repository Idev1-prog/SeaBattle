#include "Position.h"

using std::logic_error;

bool is_collision(int row) {
    return row < 1 || row > Position::_max_row;
}

bool is_collision(char col) {
    const char c = static_cast<char>(toupper(col));
    return c < 'A' || c > static_cast<char>('A' + Position::_max_col - 1);
}

Position::Position() {
    _row = rand() % _max_row;
    _col = rand() % _max_col;
}

Position::Position(int row, int col) : _row(row), _col(col) {
    if (is_collision(_row) || is_collision(static_cast<char>(_col + 'A' - 1)))
        throw logic_error("Invalid input: incorrect position");
    if (_col < 1 || _col > _max_col) throw logic_error("Invalid input: incorrect position");
}

Position::Position(int row, char col) : _row(row), _col(0) {
    if (is_collision(_row)) throw logic_error("Invalid input: incorrect position");
    if (is_collision(col)) throw logic_error("Invalid input: incorrect position");
    _col = toupper(col) - 'A' + 1;
}

Position::Position(const std::string& str) : _row(0), _col(0) {
    parse(str, *this);
}

void Position::row(int row) {
    if (is_collision(row)) throw logic_error("Invalid input: incorrect position");
    _row = row;
}

void Position::col(int col) {
    if (col < 1 || col > _max_col) throw logic_error("Invalid input: incorrect position");
    _col = col;
}

void Position::col(char col) {
    if (is_collision(col)) throw logic_error("Invalid input: incorrect position");
    _col = toupper(col) - 'A' + 1;
}

void parse(const std::string& str, Position& position) {
    int row = 0;
    char col = '\0';
    std::istringstream iss(str);

    if (!(iss >> row)) throw logic_error("Invalid input: incorrect position");
    if (!(iss >> col)) throw logic_error("Invalid input: incorrect position");

    position.row(row);
    position.col(col);
}