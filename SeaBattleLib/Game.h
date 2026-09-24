#pragma once
#include "Player.h"

class Game {
    Player _user;
    Player _computer;

    void user_init(const std::string& input);
    void computer_init(const std::string& input);

    State user_move();
    State computer_move();

    bool is_end() const noexcept;
    void show_game_window() const;
    void init();

    static bool is_hit(State s) noexcept;
    static bool parse_line(const std::string& line, Ship& ship);

public:
    Game() = default;
    void start();
};

