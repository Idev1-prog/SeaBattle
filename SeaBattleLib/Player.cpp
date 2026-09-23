#include "Player.h"

const int Player::_max_ships_counts[4] = { 4, 3, 2, 1 };

Player::Player() {
    for (int i = 0; i < 4; i++) {
        _ships_counts[i] = 0;
    }
}

void Player::set_ship(const Ship& ship) {
    int size = ship.size();
    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid input: incorrect field");
    }
    int index = size - 1;
    if (_ships_counts[index] < _max_ships_counts[index]) {
        try {
            _gamefield.set(ship);
            _ships_counts[index]++;
            return;
        }
        catch (std::exception&) {}
    }
    throw std::logic_error("Invalid input: incorrect field");
}

State Player::set_action(int row, char col) {
    State result = _gamefield.set(row, col);

    if (result == BoatDestroyed) {
        _ships_counts[0]--;
    }
    else if (result == DestroyersDestroyed) {
        _ships_counts[1]--;
    }
    else if (result == CruisersDestroyed) {
        _ships_counts[2]--;
    }
    else if (result == BattleshipDestroyed) {
        _ships_counts[3]--;
    }
    return result;
}

void Player::show_field(bool hide_ships) const {
    std::cout << to_string(_gamefield, hide_ships) << std::endl;
    std::cout << "Ships Left:" << std::endl;
    std::cout << "* - " << _ships_counts[0] << " "
        << "** - " << _ships_counts[1] << " "
        << "*** - " << _ships_counts[2] << " "
        << "**** - " << _ships_counts[3] << std::endl;
}

bool Player::check_lose() const noexcept {
    for (int i = 0; i < 4; i++) {
        if (_ships_counts[i] > 0) {
            return false;
        }
    }
    return true;
}

bool Player::check_ready() const noexcept {
    for (int i = 0; i < 4; i++) {
        if (_ships_counts[i] != _max_ships_counts[i]) {
            return false;
        }
    }
    return true;
}