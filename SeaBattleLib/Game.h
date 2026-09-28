#pragma once
#include "Player.h"
#include <vector>
#include <queue>

class Game {
private:
    Player _user;
    Player _computer;

    // расставляет корабли пользователя по введённому набору строк
    void user_init(const std::string& input);
    // генерирует случайные корабли компьютера
    void computer_init();

    State user_move();
    State computer_move();

    bool is_end() const noexcept;
    void show_game_window() const;

    static bool is_hit(State s) noexcept;
    // возвращает false, если корабль уже есть в списке - его нельзя добавлять повторно
    static bool add_ship(std::vector<std::string>& ships, const std::string& ship);

    // --- логика прицельной стрельбы компьютера ---
    struct Shot {
        int row;
        char col;
    };

    std::queue<Shot> _target_queue;   // очередь клеток для проверки вокруг подбитого корабля

    Shot random_shot() const;         // случайная клетка поля
    void add_neighbors(int row, char col); // добавить соседей клетки в очередь проверки
    void clear_logic();               // сброс логики после уничтожения корабля

public:
    Game() = default;
    void start();
};
