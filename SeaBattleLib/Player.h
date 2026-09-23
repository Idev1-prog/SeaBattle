#pragma once
#include "GameField.h"

class Player {
    GameField _gamefield;
    int _ships_counts[4];

    static const int _max_ships_counts[4];

public:
    Player();

    void set_ship(const Ship& ship);
    State set_action(int row, char col);

    void show_field(bool hide_ships = false) const;
    bool check_lose() const noexcept;
    bool check_ready() const noexcept;
};
