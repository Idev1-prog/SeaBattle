#pragma once
#include <iostream>
#include <regex>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <random>

class Position {
private:
    int _row;
    int _col;
    static const int _max_row = 10;
    static const int _max_col = 10;

public:
    Position();
    Position(int row, int col);
    Position(int row, char col);
    Position(const Position& other) = default;
    Position(const std::string& str);

    ~Position() = default;
    Position& operator=(const Position& other) = default;

    inline int row() const noexcept { return _row; }
    inline int col() const noexcept { return _col; }
    static int max_col() noexcept { return _max_col; }
    static int max_row() noexcept { return _max_row; }
    inline char char_col() const noexcept { return static_cast<char>('A' + _col - 1); }

    void row(int);
    void col(int);
    void col(char);

    friend void parse(const std::string& str, Position& position);
    friend bool is_collision(int row);
    friend bool is_collision(char col);
};

void parse(const std::string& str, Position& position);
bool is_collision(int row);
bool is_collision(char col);