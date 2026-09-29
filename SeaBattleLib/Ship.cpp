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

    // Границы поля проверяем напрямую по ЧИСЛАМ (row()/col()).
    // Нельзя оборачивать вычисления в try/catch с Position(int,int):
    // этот конструктор трактует второй аргумент как ASCII-код буквы,
    // поэтому строка вида Position(row, col + size - 1) при переполнении
    // молча создаёт "буквенную" позицию вместо исключения — проверка
    // никогда не срабатывала бы корректно.
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