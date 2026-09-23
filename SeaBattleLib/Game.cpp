#include "Game.h"


bool Game::parse_line(const std::string& line, Ship& ship) {
    if (line.empty()) {
        return false;
    }
    int row = 0;
    char col = 0;
    char dir = 0;
    int size = 0;
    std::istringstream ss(line);
    if (!(ss >> size >> dir >> row >> col)) {
        throw std::logic_error("Invalid input: incorrect field");
    }
    ship = Ship(size, dir, row, col);
    return true;
}


void Game::user_init(const std::string& input) {
    std::istringstream ss(input);
    std::string line;
    while (std::getline(ss, line)) {
        Ship ship(1, 'H', 1, 'A');
        if (parse_line(line, ship)) {
            _user.set_ship(ship);
        }
    }
    if (!_user.check_ready()) {
        throw std::logic_error("Invalid input: incorrect field");
    }
}

void Game::computer_init(const std::string& input) {
    std::istringstream ss(input);
    std::string line;
    while (std::getline(ss, line)) {
        Ship ship(1, 'H', 1, 'A');
        if (parse_line(line, ship)) {
            _computer.set_ship(ship);
        }
    }
    if (!_computer.check_ready()) {
        throw std::logic_error("Invalid input: incorrect field");
    }
}

State Game::user_move() {
    std::cout << "Your move (row col): ";
    std::string line;
    std::getline(std::cin, line);

    std::istringstream ss(line);
    int row = 0;
    char col = 0;
    if (!(ss >> row >> col)) {
        throw std::logic_error("Invalid input: incorrect move");
    }
    return _computer.set_action(row, col);
}

State Game::computer_move() {
    static int cell = 0;

    while (true) {
        int r = cell / 10;
        int c = cell % 10;
        cell++;

        State s = Missed;
        try {
            s = _user.set_action(r + 1, static_cast<char>('A' + c));
        }
        catch (std::logic_error&) {
            continue;
        }
        return s;
    }
}

bool Game::is_end() const noexcept {
    return _user.check_lose() || _computer.check_lose();
}

bool Game::is_hit(State s) noexcept {
    return s == Hit || s == BoatDestroyed || s == DestroyersDestroyed
        || s == CruisersDestroyed || s == BattleshipDestroyed;
}

void Game::show_game_window() const {
    std::cout << "= COMPUTER GAME FIELD =" << std::endl << std::endl;
    _computer.show_field(true);
    std::cout << std::endl;
    std::cout << "=== YOUR PLAY FIELD ===" << std::endl << std::endl;
    _user.show_field(false);
    std::cout << std::endl;
}

void Game::start() {
    std::string input;

    std::getline(std::cin, input);
    user_init(input);

    std::getline(std::cin, input);
    std::getline(std::cin, input);
    computer_init(input);

    std::cout << "Game started!" << std::endl << std::endl;

    while (!is_end()) {
        State s = user_move();
        while (is_hit(s)) {
            if (is_end()) break;
            std::cout << ">>> Hit! Your turn again." << std::endl;
            s = user_move();
        }
        if (is_end()) break;

        std::cout << ">>> Computer's move..." << std::endl;
        s = computer_move();
        while (is_hit(s)) {
            if (is_end()) break;
            std::cout << ">>> Computer hit! It moves again." << std::endl;
            s = computer_move();
        }
    }

    std::cout << std::endl;
    show_game_window();

    if (_computer.check_lose()) {
        std::cout << "USER WIN!" << std::endl;
    }
    else {
        std::cout << "COMPUTER WIN!" << std::endl;
    }
}
