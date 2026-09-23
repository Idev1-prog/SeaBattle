#include "GameField.h"

using std::logic_error;
using std::string;


GameField::GameField() : _n(10), _m(10) {
    _field = new char* [_n];
    for (int i = 0; i < _n; i++) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; j++) {
            _field[i][j] = ' ';
        }
    }
}

GameField::~GameField() {
    for (int i = 0; i < _n; i++) {
        delete[] _field[i];
    }
    delete[] _field;
}


void GameField::set(const Ship& ship) {
    if (is_collision(*this, ship)) {
        throw logic_error("Invalid input: incorrect field");
    }

    int row = ship.row() - 1;
    int col = ship.col() - 1;

    for (int i = 0; i < ship.size(); i++) {
        if (ship.direction() == Direction::Horizontal) {
            _field[row][col + i] = '*';
        }
        else {
            _field[row + i][col] = '*';
        }
    }
}


State GameField::set(int row, char col) {
    col = static_cast<char>(toupper(col));

    if (row < 1 || row > _n || col < 'A' || col >= 'A' + _m) {
        throw logic_error("Invalid input: incorrect move");
    }

    int r = row - 1;
    int c = col - 'A';

    if (_field[r][c] == '.' || _field[r][c] == 'X') {
        throw logic_error("Invalid input: incorrect move");
    }

    if (_field[r][c] == ' ') {
        _field[r][c] = '.';
        return State::Missed;
    }


    _field[r][c] = 'X';

    int size = check_destroy(r, c);
    switch (size) {
    case 1: return State::BoatDestroyed;
    case 2: return State::DestroyersDestroyed;
    case 3: return State::CruisersDestroyed;
    case 4: return State::BattleshipDestroyed;
    default: return State::Hit;
    }
}


int GameField::check_destroy(int row, int col) {
    int left = col, right = col;
    while (left > 0 && (_field[row][left - 1] == 'X' || _field[row][left - 1] == '*')) left--;
    while (right < _m - 1 && (_field[row][right + 1] == 'X' || _field[row][right + 1] == '*')) right++;

    int up = row, down = row;
    while (up > 0 && (_field[up - 1][col] == 'X' || _field[up - 1][col] == '*')) up--;
    while (down < _n - 1 && (_field[down + 1][col] == 'X' || _field[down + 1][col] == '*')) down++;

    int size = 1;
    bool destroyed = true;

    if (right > left) {
        size = right - left + 1;
        for (int c = left; c <= right; c++) {
            if (_field[row][c] == '*') destroyed = false;
        }
    }
    else if (down > up) {
        size = down - up + 1;
        for (int r = up; r <= down; r++) {
            if (_field[r][col] == '*') destroyed = false;
        }
    }

    return destroyed ? size : 0;
}


string to_string(const GameField& field, bool show) {
    string result = "  |";


    for (int j = 0; j < field._m; j++) {
        result += static_cast<char>('A' + j);
        if (j < field._m - 1) result += ' ';
    }
    result += "|\n";


    result += "  +";
    for (int j = 0; j < field._m * 2 - 1; j++) {
        result += '-';
    }
    result += "+\n";


    for (int i = 0; i < field._n; i++) {
        result += std::to_string(i + 1);
        if (i + 1 < 10) result += ' ';
        result += '|';

        for (int j = 0; j < field._m; j++) {
            char cell = field._field[i][j];
            if (!show && cell == '*') cell = ' ';
            result += cell;
            if (j < field._m - 1) result += '|';
        }
        result += "|\n";
    }


    result += "  +";
    for (int j = 0; j < field._m * 2 - 1; j++) {
        result += '-';
    }
    result += "+\n";

    return result;
}

bool is_collision(const GameField& field, const Ship& ship) {
    int row = ship.row() - 1;
    int col = ship.col() - 1;
    int size = ship.size();

    if (row < 0 || row >= field._n || col < 0 || col >= field._m || size < 1 || size > 4) {
        return true;
    }
    if (ship.direction() == Direction::Horizontal && col + size > field._m) return true;
    if (ship.direction() == Direction::Vertical && row + size > field._n) return true;

    int min_r = row - 1;
    int max_r = (ship.direction() == Direction::Vertical) ? (row + size) : (row + 1);
    int min_c = col - 1;
    int max_c = (ship.direction() == Direction::Horizontal) ? (col + size) : (col + 1);

    for (int r = min_r; r <= max_r; r++) {
        for (int c = min_c; c <= max_c; c++) {
            if (r >= 0 && r < field._n && c >= 0 && c < field._m) {
                if (field._field[r][c] == '*' || field._field[r][c] == 'X') {
                    return true;
                }
            }
        }
    }

    return false;
}