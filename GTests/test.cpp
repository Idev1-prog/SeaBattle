#include "pch.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <cctype>

#include <Position.h>
#include <Player.h>
#include <GameField.h>
#include <Game.h>


// AI TESTS:

// Возвращает символ клетки поля (row 1..10, col 'A'..'J') из вывода
// to_string(field, true). Формат строки данных: "|c|c|...|c|" — клетка j
// находится между (j+1)-м и (j+2)-м символами '|'.
char get_cell(const GameField& field, int row, char col) {
    std::string dump = to_string(field, true);
    size_t start = 0;
    for (int i = 0; i < row + 1; ++i)     // шаг: строка заголовка "  |A B..|", строка границы "  +---+", затем N строк данных
        start = dump.find('\n', start) + 1;
    size_t left = dump.find('|', start);   // '|' перед колонкой A
    for (int j = 0; j < col - 'A'; ++j)
        left = dump.find('|', left + 1);
    return dump[left + 1];
}

#define CHECK_CELL(field, row, col, expected) EXPECT_EQ(get_cell(field, row, col), (expected))

//POSITION

TEST(PositionTest, DefaultConstructorInsideField) {
    // Конструктор по умолчанию использует rand() (значения 1..10), проверяем границы
    for (int i = 0; i < 50; ++i) {
        Position p;
        EXPECT_GE(p.row(), 1);
        EXPECT_LE(p.row(), 10);
        EXPECT_GE(p.col(), 1);
        EXPECT_LE(p.col(), 10);
    }
}

TEST(PositionTest, IntIntConstructorValid) {
    Position p(1, 1);
    EXPECT_EQ(p.row(), 1);
    EXPECT_EQ(p.col(), 1);

    Position p2(10, 10);
    EXPECT_EQ(p2.row(), 10);
    EXPECT_EQ(p2.col(), 10);
    EXPECT_EQ(p2.char_col(), 'J');
}

TEST(PositionTest, IntIntConstructorThrowsOutOfRange) {
    EXPECT_THROW(Position(0, 1), std::logic_error);
    EXPECT_THROW(Position(11, 1), std::logic_error);
    EXPECT_THROW(Position(1, 0), std::logic_error);
    EXPECT_THROW(Position(1, 11), std::logic_error);
}

TEST(PositionTest, IntCharConstructorAcceptsLowercase) {
    Position upper(5, 'B');
    Position lower(5, 'b');
    EXPECT_EQ(upper.row(), lower.row());
    EXPECT_EQ(upper.col(), lower.col());
    EXPECT_EQ(upper.char_col(), 'B');
}

TEST(PositionTest, IntCharConstructorThrowsOutOfRange) {
    EXPECT_THROW(Position(0, 'A'), std::logic_error);
    EXPECT_THROW(Position(11, 'A'), std::logic_error);
    EXPECT_THROW(Position(1, '@'), std::logic_error); // символ до 'A'
    EXPECT_THROW(Position(1, 'K'), std::logic_error); // после J (на поле 10 колонок)
}

TEST(PositionTest, StringConstructorParsesRowCol) {
    Position p(std::string("7C"));
    EXPECT_EQ(p.row(), 7);
    EXPECT_EQ(p.col(), 3);
    EXPECT_EQ(p.char_col(), 'C');
}

TEST(PositionTest, SettersAndGetters) {
    Position p(1, 'A');
    p.row(5);
    p.col(9);
    EXPECT_EQ(p.row(), 5);
    EXPECT_EQ(p.col(), 9);
    EXPECT_EQ(p.char_col(), 'I');

    p.col('B');
    EXPECT_EQ(p.col(), 2);
}

TEST(PositionTest, SettersThrowOutOfRange) {
    Position p(1, 'A');
    EXPECT_THROW(p.row(0), std::logic_error);
    EXPECT_THROW(p.row(11), std::logic_error);
    EXPECT_THROW(p.col(0), std::logic_error);
    EXPECT_THROW(p.col(11), std::logic_error);
    EXPECT_THROW(p.col('Z'), std::logic_error);
}

TEST(PositionTest, CopyConstructorAndAssignment) {
    Position a(4, 'D');
    Position b(a);
    Position c(1, 'A');
    c = a;
    EXPECT_EQ(b.row(), 4);
    EXPECT_EQ(b.col(), 4);
    EXPECT_EQ(c.row(), a.row());
    EXPECT_EQ(c.col(), a.col());
}

TEST(PositionTest, StaticLimits) {
    EXPECT_EQ(Position::max_row(), 10);
    EXPECT_EQ(Position::max_col(), 10);
}

TEST(PositionTest, ParseFunction) {
    Position p(1, 'A');
    parse("3E", p);
    EXPECT_EQ(p.row(), 3);
    EXPECT_EQ(p.col(), 5);

    EXPECT_THROW(parse("abc", p), std::logic_error); // нет числа
    EXPECT_THROW(parse("5", p), std::logic_error);   // нет буквы
    EXPECT_THROW(parse("11A", p), std::logic_error); // row вне поля
}

// Блок тестов: SHIP

TEST(ShipTest, MainConstructorStoresParameters) {
    Ship ship(3, Position(2, 'B'), Direction::Vertical);
    EXPECT_EQ(ship.size(), 3);
    EXPECT_EQ(ship.row(), 2);
    EXPECT_EQ(ship.col(), 2);
    EXPECT_EQ(ship.direction(), Direction::Vertical);
    EXPECT_EQ(ship.position().char_col(), 'B');
}

TEST(ShipTest, CharDirectionConstructor) {
    Ship h(2, 'h', 1, 'A');
    EXPECT_EQ(h.direction(), Direction::Horizontal);

    Ship v(2, 'V', 1, 'A');
    EXPECT_EQ(v.direction(), Direction::Vertical);
}

TEST(ShipTest, ConstructorThrowsOnInvalidInput) {
    Position ok(1, 'A');
    EXPECT_THROW(Ship(0, ok, Direction::Horizontal), std::logic_error);
    EXPECT_THROW(Ship(5, ok, Direction::Horizontal), std::logic_error);
    EXPECT_THROW(Ship(2, 'X', 1, 'A'), std::logic_error);               // неверное направление
    EXPECT_THROW(Ship(3, Position(9, 'A'), Direction::Vertical), std::logic_error);   // вылезает за нижний край
}

TEST(ShipTest, StringConstructorParse) {
    Ship ship("2 V 5 C");
    EXPECT_EQ(ship.size(), 2);
    EXPECT_EQ(ship.direction(), Direction::Vertical);
    EXPECT_EQ(ship.row(), 5);
    EXPECT_EQ(ship.col(), 3);

    EXPECT_THROW(Ship("2 H 1"), std::logic_error);      // не хватает колонки
    EXPECT_THROW(Ship("H 1 A"), std::logic_error);      // нет размера
    EXPECT_THROW(Ship("2 X 1 A"), std::logic_error);    // неверное направление
    EXPECT_THROW(Ship("garbage"), std::logic_error);
}

TEST(ShipTest, SizeSetter) {
    Ship ship(2, Position(1, 'A'), Direction::Horizontal);
    ship.size(4);
    EXPECT_EQ(ship.size(), 4);

    EXPECT_THROW(ship.size(5), std::logic_error);
    EXPECT_THROW(ship.size(0), std::logic_error);
}

TEST(ShipTest, SizeIncreaseBeyondEdgeThrows) {
    Ship edge(3, Position(1, 'H'), Direction::Horizontal); // занимает H,I,J
    EXPECT_NO_THROW(edge.size(3));
    EXPECT_THROW(edge.size(4), std::logic_error);          // H..K — выход за поле
}

TEST(ShipTest, RowColSetters) {
    Ship ship(2, Position(1, 'A'), Direction::Horizontal);
    ship.row(5);
    EXPECT_EQ(ship.row(), 5);
    ship.col('C');
    EXPECT_EQ(ship.col(), 3);
    ship.col(7); // числовая перегрузка: значения >= 'A' трактуются как буквенные коды
    EXPECT_EQ(ship.col(), 7); // 7 < 'A', значит это просто номер колонки
    EXPECT_EQ(ship.position().char_col(), 'G');

    EXPECT_THROW(ship.row(0), std::logic_error);
    EXPECT_THROW(ship.row(11), std::logic_error);
    EXPECT_THROW(ship.col('K'), std::logic_error);
}

TEST(ShipTest, MoveIntoOutOfBoundsThrows) {
    Ship ship(3, Position(1, 'H'), Direction::Horizontal);
    EXPECT_THROW(ship.col('I'), std::logic_error);
    EXPECT_NO_THROW(ship.col('A'));
}

TEST(ShipTest, DirectionSetter) {
    Ship ship(2, Position(5, 'J'), Direction::Vertical);
    EXPECT_THROW(ship.direction(Direction::Horizontal), std::logic_error);
    EXPECT_NO_THROW(ship.direction(Direction::Vertical));
    EXPECT_EQ(ship.direction(), Direction::Vertical);

    EXPECT_THROW(ship.direction('Q'), std::logic_error);
}

TEST(ShipTest, PositionSetterMovesShip) {
    Ship ship(2, Position(1, 'A'), Direction::Horizontal);
    EXPECT_NO_THROW(ship.position(Position(3, 'C')));
    EXPECT_EQ(ship.row(), 3);
    EXPECT_EQ(ship.col(), 3);
}

TEST(ShipTest, ShipCanExtendBeyondFieldEdge) {
    EXPECT_THROW(Ship(2, Position(10, 'J'), Direction::Horizontal), std::logic_error);
    EXPECT_THROW(Ship(3, Position(9, 'A'), Direction::Vertical), std::logic_error);

    EXPECT_NO_THROW(Ship(3, Position(1, 'H'), Direction::Horizontal));
    EXPECT_NO_THROW(Ship(3, Position(8, 'A'), Direction::Vertical));
}

TEST(ShipTest, CopyConstructorAndAssignment) {
    Ship a(3, Position(2, 'B'), Direction::Vertical);
    Ship b(a);
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b.row(), 2);
    EXPECT_EQ(b.col(), 2);
    EXPECT_EQ(b.direction(), Direction::Vertical);

    Ship c(1, Position(1, 'A'), Direction::Horizontal);
    c = a;
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c.direction(), Direction::Vertical);
}

// Блок тестов: GAMEFIELD

TEST(GameFieldTest, InitiallyEmpty) {
    GameField field;
    CHECK_CELL(field, 1, 'A', ' ');  // строка 1, колонка A
    CHECK_CELL(field, 10, 'J', ' ');  // строка 10, колонка J
}

TEST(GameFieldTest, SetShipMarksCells) {
    GameField field;
    field.set(Ship(3, Position(1, 'A'), Direction::Horizontal));
    CHECK_CELL(field, 1, 'A', '*');
    CHECK_CELL(field, 1, 'B', '*');
    CHECK_CELL(field, 1, 'C', '*');
    CHECK_CELL(field, 1, 'D', ' ');

    field.set(Ship(2, Position(5, 'F'), Direction::Vertical));
    CHECK_CELL(field, 5, 'F', '*');
    CHECK_CELL(field, 6, 'F', '*');
    CHECK_CELL(field, 7, 'F', ' ');
}

TEST(GameFieldTest, SetShipCollisionThrows) {
    GameField field;
    field.set(Ship(2, Position(1, 'A'), Direction::Horizontal));
    EXPECT_THROW(field.set(Ship(1, Position(1, 'B'), Direction::Horizontal)), std::logic_error); // занятая клетка
    EXPECT_THROW(field.set(Ship(1, Position(2, 'A'), Direction::Horizontal)), std::logic_error); // диагональный сосед — по правилам нельзя
    EXPECT_NO_THROW(field.set(Ship(1, Position(1, 'D'), Direction::Horizontal))); // через одну клетку — можно
}

TEST(GameFieldTest, ShotMissed) {
    GameField field;
    EXPECT_EQ(field.set(3, 'C'), State::Missed);
    CHECK_CELL(field, 3, 'C', '.');
}

TEST(GameFieldTest, ShotHitThenDestroyTwoDeckShip) {
    GameField field;
    field.set(Ship(2, Position(1, 'A'), Direction::Horizontal));

    EXPECT_EQ(field.set(1, 'A'), State::Hit);
    CHECK_CELL(field, 1, 'A', 'X');

    EXPECT_EQ(field.set(1, 'B'), State::DestroyersDestroyed);
    CHECK_CELL(field, 1, 'B', 'X');
}

TEST(GameFieldTest, ShotDestroysThreeAndFourDeckShips) {
    GameField field;
    field.set(Ship(3, Position(1, 'A'), Direction::Horizontal));
    field.set(Ship(4, Position(3, 'A'), Direction::Vertical));

    EXPECT_EQ(field.set(1, 'A'), State::Hit);
    EXPECT_EQ(field.set(1, 'B'), State::Hit);
    EXPECT_EQ(field.set(1, 'C'), State::CruisersDestroyed);

    EXPECT_EQ(field.set(3, 'A'), State::Hit);
    EXPECT_EQ(field.set(4, 'A'), State::Hit);
    EXPECT_EQ(field.set(5, 'A'), State::Hit);
    EXPECT_EQ(field.set(6, 'A'), State::BattleshipDestroyed);
}

TEST(GameFieldTest, OneDeckShipDestroyReturnsBoatDestroyed) {
    // 1-палубник: check_destroy возвращает 1 -> BoatDestroyed
    GameField field;
    field.set(Ship(1, Position(1, 'A'), Direction::Horizontal));
    EXPECT_EQ(field.set(1, 'A'), State::BoatDestroyed);
}

TEST(GameFieldTest, ShipsSeparatedByGapAreIndependent) {
    GameField field;
    field.set(Ship(2, Position(1, 'A'), Direction::Horizontal)); // A1,B1
    field.set(Ship(2, Position(1, 'D'), Direction::Horizontal)); // D1,E1

    EXPECT_EQ(field.set(1, 'D'), State::Hit);
    EXPECT_EQ(field.set(1, 'E'), State::DestroyersDestroyed);

    EXPECT_EQ(field.set(1, 'A'), State::Hit);
    EXPECT_EQ(field.set(1, 'B'), State::DestroyersDestroyed);
}

TEST(GameFieldTest, ShotOutOfBoundsThrows) {
    GameField field;
    EXPECT_THROW(field.set(0, 'A'), std::logic_error);
    EXPECT_THROW(field.set(11, 'A'), std::logic_error);
    EXPECT_THROW(field.set(1, '@'), std::logic_error);
    EXPECT_THROW(field.set(1, 'K'), std::logic_error);
}

TEST(GameFieldTest, RepeatShotThrows) {
    GameField field;
    field.set(Ship(1, Position(1, 'A'), Direction::Horizontal));
    field.set(1, 'A'); // X
    field.set(2, 'B'); // .
    EXPECT_THROW(field.set(1, 'A'), std::logic_error); // повтор в X
    EXPECT_THROW(field.set(2, 'B'), std::logic_error); // повтор в .
}

TEST(GameFieldTest, ShotLowercaseColumnWorks) {
    GameField field;
    EXPECT_EQ(field.set(3, 'c'), State::Missed); // toupper внутри set
}

TEST(GameFieldTest, ToStringHidesShipsByDefault) {
    GameField field;
    field.set(Ship(2, Position(1, 'A'), Direction::Horizontal));

    std::string hidden = to_string(field, false);
    std::string shown = to_string(field, true);

    EXPECT_NE(hidden.find('A'), std::string::npos); // есть шапка с координатами
    EXPECT_NE(shown.find('*'), std::string::npos);  // с show=true видно '*'
    EXPECT_EQ(hidden.find('*'), std::string::npos); // без show '*' скрыт
}

// Блок тестов: PLAYER

TEST(PlayerTest, NewPlayerNotReady) {
    Player p;
    EXPECT_FALSE(p.check_ready());
}

TEST(PlayerTest, FullFleetMakesPlayerReady) {
    Player p;
    p.set_ship(Ship("4 H 1 A"));
    p.set_ship(Ship("3 H 3 A"));
    p.set_ship(Ship("3 H 3 E"));
    p.set_ship(Ship("2 H 5 A"));
    p.set_ship(Ship("2 H 5 D"));
    p.set_ship(Ship("2 H 5 G"));
    p.set_ship(Ship("1 H 7 A"));
    p.set_ship(Ship("1 H 7 C"));
    p.set_ship(Ship("1 H 7 E"));
    p.set_ship(Ship("1 H 7 G"));

    EXPECT_TRUE(p.check_ready());
    EXPECT_FALSE(p.check_lose());
}

TEST(PlayerTest, SetShipRejectsDuplicateSizeOverLimit) {
    Player p;
    p.set_ship(Ship("4 H 1 A"));
    // второй 4-палубник запрещён (_max_ships_counts[3] == 1)
    EXPECT_THROW(p.set_ship(Ship("4 H 5 A")), std::logic_error);
}

TEST(PlayerTest, SetShipRejectsCollisionWithOwnField) {
    Player p;
    p.set_ship(Ship("2 H 1 A"));
    EXPECT_THROW(p.set_ship(Ship("1 H 1 A")), std::logic_error);
}

TEST(PlayerTest, SetActionDecrementsCountersOnDestroy) {
    Player p;
    p.set_ship(Ship("2 H 1 A")); // один destroyer

    EXPECT_EQ(p.set_action(1, 'A'), State::Hit);
    EXPECT_EQ(p.set_action(1, 'B'), State::DestroyersDestroyed);

    // после уничтожения единственного корабля проигрыш
    EXPECT_TRUE(p.check_lose());
}

TEST(PlayerTest, EmptyPlayerReportsLose) {
    Player p;
    EXPECT_FALSE(p.check_ready());
    EXPECT_TRUE(p.check_lose());

    Game game;
    std::istringstream in("2 H 1 A\n\n"); // неполный флот
    std::streambuf* old = std::cin.rdbuf(in.rdbuf());
    testing::internal::CaptureStdout();
    EXPECT_THROW(game.start(), std::logic_error); // игра не стартует вовсе
    testing::internal::GetCapturedStdout();
    std::cin.rdbuf(old);
}

TEST(PlayerTest, SetActionThrowsOnWrongInput) {
    Player p;
    EXPECT_THROW(p.set_action(0, 'A'), std::logic_error);
    EXPECT_THROW(p.set_action(1, 'K'), std::logic_error);
}

TEST(PlayerTest, ShowFieldPrintsSummary) {
    Player p;
    p.set_ship(Ship("2 H 1 A"));
    testing::internal::CaptureStdout();
    p.show_field(true);
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("Ships Left:"), std::string::npos);
}

// Блок тестов: GAME

TEST(GameTest, StartWithInvalidShipInputThrows) {
    Game game;
    std::istringstream in("garbage input\n");
    std::streambuf* old = std::cin.rdbuf(in.rdbuf());
    testing::internal::CaptureStdout();
    EXPECT_THROW(game.start(), std::logic_error);
    testing::internal::GetCapturedStdout();
    std::cin.rdbuf(old);
}

TEST(GameTest, StartWithWrongShipCountsThrows) {
    Game game;
    std::istringstream in("4 H 1 A\n4 H 1 A\n\n");
    std::streambuf* old = std::cin.rdbuf(in.rdbuf());
    testing::internal::CaptureStdout();
    EXPECT_THROW(game.start(), std::logic_error);
    testing::internal::GetCapturedStdout();
    std::cin.rdbuf(old);
}

TEST(GameTest, StartWithIncompleteFleetThrows) {
    Game game;
    // корректный корабль, но неполный флот -> user_init бросит "incorrect field"
    std::istringstream in("4 H 1 A\n\n");
    std::streambuf* old = std::cin.rdbuf(in.rdbuf());
    testing::internal::CaptureStdout();
    EXPECT_THROW(game.start(), std::logic_error);
    testing::internal::GetCapturedStdout();
    std::cin.rdbuf(old);
}

TEST(GameTest, StartRunsAndAsksForMove) {
    // Полный корректный флот -> игра стартует и запрашивает ход пользователя.
    // После первого выстрела stdin заканчивается (EOF) -> Game::user_move
    // бросает logic_error (отсутствие ввода = ошибка). Проверяем, что вывод
    // содержит "Game started!" и приглашение сделать ход.
    std::string full_input =
        "4 H 1 A\n3 H 3 A\n3 H 3 E\n2 H 5 A\n2 H 5 D\n2 H 5 G\n"
        "1 H 7 A\n1 H 7 C\n1 H 7 E\n1 H 7 G\n\n"
        "1 A\n";

    Game game;
    std::istringstream in(full_input);
    std::streambuf* old = std::cin.rdbuf(in.rdbuf());
    testing::internal::CaptureStdout();
    EXPECT_THROW(game.start(), std::logic_error); // EOF после первого хода
    std::string out = testing::internal::GetCapturedStdout();
    std::cin.rdbuf(old);

    EXPECT_NE(out.find("Game started!"), std::string::npos);
    EXPECT_NE(out.find("Your move"), std::string::npos);
}