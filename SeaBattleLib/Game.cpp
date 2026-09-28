#include "Game.h"
//#include <cstdlib> // rand(), srand()
//#include <ctime>   // time()

bool Game::add_ship(std::vector<std::string>& ships, const std::string& ship) {
    for (const auto& s : ships) {
        if (s == ship) {
            return false; // такой корабль уже есть в списке
        }
    }
    ships.push_back(ship);
    return true;
}

void Game::user_init(const std::string& input) {
    std::istringstream ss(input);
    std::string line;

    while (std::getline(ss, line)) {
        if (line.empty()) {
            continue;
        }
        Ship ship(line);
        _user.set_ship(ship);
    }

    if (!_user.check_ready()) {
        throw std::logic_error("Invalid input: incorrect field");
    }
}

void Game::computer_init() {
    // стандартный набор кораблей: 1x4, 2x3, 3x2, 4x1
    const int sizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int size : sizes) {
        bool placed = false;
        while (!placed) {
            // rand() % n даёт число от 0 до n-1, поэтому +1 - клетки поля нумеруются от 1
            int row = rand() % Position::max_row() + 1;
            int col = rand() % Position::max_col() + 1;
            Direction dir = (rand() % 2 == 0) ? Horizontal : Vertical;

            // конструктор Ship сам проверит выход за поле,
            // а set_ship - столкновения с уже стоящими кораблями
            try {
                _computer.set_ship(Ship(size, Position(row, col), dir));
                placed = true;
            }
            catch (std::logic_error&) {
                continue;
            }
        }
    }
}

State Game::user_move() {
    // ход продолжается до тех пор, пока пользователь не введёт корректный выстрел
    while (true) {
        std::string line;
        std::cout << "Your move (e.g. 5B): ";
        if (!std::getline(std::cin, line)) {
            throw std::logic_error("Invalid input: incorrect move"); // конец потока ввода
        }

        std::istringstream ss(line);
        int row = 0;
        char col = 0;
        if (!(ss >> row >> col)) {
            std::cout << ">>> Invalid input: incorrect move. Try again." << std::endl;
            continue;
        }

        try {
            return _computer.set_action(row, col);
        }
        catch (std::logic_error&) {
            std::cout << ">>> Invalid input: incorrect move. Try again." << std::endl;
        }
    }
}

Game::Shot Game::random_shot() const {
    // случайная клетка поля: rand() % (10 * 10) даёт число от 0 до 99
    // (уже посещённые клетки отфильтрует set_action в computer_move)
    int cell = rand() % (Position::max_row() * Position::max_col());
    Shot shot;
    shot.row = cell / Position::max_col() + 1;
    shot.col = static_cast<char>('A' + cell % Position::max_col());
    return shot;
}

void Game::add_neighbors(int row, char col) {
    const int dr[] = { -1, 1, 0, 0 };
    const int dc[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; i++) {
        int r = row + dr[i];
        int c = (col - 'A') + dc[i];

        if (r >= 1 && r <= Position::max_row() && c >= 0 && c < Position::max_col()) {
            Shot s{ r, static_cast<char>('A' + c) };
            _target_queue.push(s);
        }
    }
}

void Game::clear_logic() {
    while (!_target_queue.empty()) _target_queue.pop();
}

State Game::computer_move() {
    // если есть подбитый, но ещё не уничтоженный корабль - исследуем его окрестности
    while (!_target_queue.empty()) {
        Shot s = _target_queue.front();
        _target_queue.pop();

        State result;
        try {
            result = _user.set_action(s.row, s.col);
        }
        catch (std::logic_error&) {
            continue; // клетка уже посещена - пропускаем
        }

        if (result == State::Hit) {
            add_neighbors(s.row, s.col);
        }
        else if (is_hit(result)) { // корабль уничтожен - логика больше не нужна
            clear_logic();
        }
        return result;
    }

    // обычный случайный выстрел
    while (true) {
        Shot s = random_shot();

        State result;
        try {
            result = _user.set_action(s.row, s.col);
        }
        catch (std::logic_error&) {
            continue; // клетка уже посещена - стреляем в другую
        }

        if (result == State::Hit) {
            add_neighbors(s.row, s.col);
        }
        else if (is_hit(result)) {
            clear_logic();
        }
        return result;
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

    std::cout << "You need to place the following ships on the field:" << std::endl;
    std::cout << "1 ship of size 4, 2 ships of size 3, 3 ships of size 2, 4 ships of size 1." << std::endl;
    std::cout << "Enter each ship as \"size direction row col\" (e.g. 4 H 1 A)," << std::endl;
    std::cout << "one per line. Type an empty line when you are done:" << std::endl;

    std::vector<std::string> battleship;   // корабли размера 4
    std::vector<std::string> cruiser;     // корабли размера 3
    std::vector<std::string> destroyer;    // корабли размера 2
    std::vector<std::string> boat;         // корабли размера 1

    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        Ship ship(line); // выбросит исключение при неверном формате

        switch (ship.size()) {
        case 4:
            if (!add_ship(battleship, line) || battleship.size() > 1) {
                throw std::logic_error("Invalid input: incorrect field");
            }
            break;
        case 3:
            if (!add_ship(cruiser, line) || cruiser.size() > 2) {
                throw std::logic_error("Invalid input: incorrect field");
            }
            break;
        case 2:
            if (!add_ship(destroyer, line) || destroyer.size() > 3) {
                throw std::logic_error("Invalid input: incorrect field");
            }
            break;
        case 1:
            if (!add_ship(boat, line) || boat.size() > 4) {
                throw std::logic_error("Invalid input: incorrect field");
            }
            break;
        default:
            throw std::logic_error("Invalid input: incorrect field");
        }

        input += line + "\n";
    }

    user_init(input);
    computer_init();

    std::cout << std::endl << "Game started!" << std::endl << std::endl;

    // показываем стартовое поле перед первым ходом
    show_game_window();

    while (!is_end()) {
        // ход пользователя, при попадании - повторный ход
        State s = user_move();
        show_game_window();
        while (is_hit(s) && !is_end()) {
            std::cout << ">>> Hit! Your turn again." << std::endl;
            s = user_move();
            show_game_window();
        }
        if (is_end()) {
            break;
        }

        // ход компьютера, при попадании - повторный ход
        std::cout << ">>> Computer's move..." << std::endl;
        s = computer_move();
        show_game_window();
        while (is_hit(s) && !is_end()) {
            std::cout << ">>> Computer hit! It moves again." << std::endl;
            s = computer_move();
            show_game_window();
        }
    }

    std::cout << std::endl;

    if (_computer.check_lose()) {
        std::cout << "USER WIN!" << std::endl;
    }
    else {
        std::cout << "COMPUTER WIN!" << std::endl;
    }
}
