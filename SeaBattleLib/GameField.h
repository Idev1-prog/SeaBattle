#pragma once
#include "Ship.h"

enum State { Missed, BoatDestroyed, DestroyersDestroyed, CruisersDestroyed, BattleshipDestroyed, Hit };

class GameField {
    char** _field;
    const int _n;
    const int _m;

public:
    GameField();
    ~GameField();

    void set(const Ship& ship);
    State set(int row, char col);
    int check_destroy(int row, int col);

    friend std::string to_string(const GameField& field, bool show);
    friend bool is_collision(const GameField& field, const Ship& ship);
};

std::string to_string(const GameField& field, bool show = false);