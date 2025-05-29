// liadi.haim@gmail.com 
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"
#include "Baron.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <stdexcept>

extern std::unique_ptr<Player> createPlayer(const std::string& name, const std::string& role);
// Factory function to create players based on their role
TEST_CASE("Game Initialization and Player Management") {
    Game game;

    SUBCASE("Add valid players") {
        game.addPlayer(createPlayer("Alice", "Governor"));
        game.addPlayer(createPlayer("Bob", "Spy"));
        CHECK(game.getActivePlayers().size() == 2);
    }

    SUBCASE("Reject duplicate names or invalid roles") {
        game.addPlayer(createPlayer("Alice", "Governor"));
        CHECK_THROWS(game.addPlayer(createPlayer("Alice", "Spy")));
        CHECK_THROWS(game.addPlayer(createPlayer("Charlie", "InvalidRole")));
    }
}
// Test case for adding players to the game and checking active players
TEST_CASE("Turn Rotation and Actions") {
    Game game;
    game.addPlayer(createPlayer("Alice", "Governor"));
    game.addPlayer(createPlayer("Bob", "Spy"));

    CHECK(game.turn() == "Alice");
    game.gather("Alice");
    game.nextTurn();
    CHECK(game.turn() == "Bob");
    game.gather("Bob");
    game.nextTurn();
    CHECK(game.turn() == "Alice");
}
// Test case for turn rotation and gathering coins
TEST_CASE("Economic Actions - Gather and Tax") {
    Game game;
    game.addPlayer(createPlayer("Gov", "Governor"));
    game.addPlayer(createPlayer("Normal", "Spy"));

    SUBCASE("Gather") {
        int coins = game.getPlayerCoins("Gov");
        game.gather("Gov");
        CHECK(game.getPlayerCoins("Gov") == coins + 1);
    }

    SUBCASE("Tax") {
        int govCoins = game.getPlayerCoins("Gov");
        int spyCoins = game.getPlayerCoins("Normal");
        game.tax("Gov");
        game.nextTurn();
        game.tax("Normal");
        CHECK(game.getPlayerCoins("Gov") == govCoins + 3);
        CHECK(game.getPlayerCoins("Normal") == spyCoins + 2);
    }
}
// Test case for economic actions like gathering and taxing coins
TEST_CASE("Spy Role Abilities") {
    Game game;
    game.addPlayer(createPlayer("Spy", "Spy"));
    game.addPlayer(createPlayer("Target", "Baron"));

    for (int i = 0; i < 3; ++i) game.gather("Target");

    SUBCASE("Spy on Player") {
        int coins = game.spyAction("Spy", "Target");
        CHECK(coins == game.getPlayerCoins("Target"));
    }

    SUBCASE("Prevent Arrest") {
        game.preventArrest("Spy", "Target");
        game.nextTurn();
        CHECK_THROWS(game.arrest("Target", "Spy"));
    }
}

TEST_CASE("Merchant Role Abilities") {
    Game game;
    game.addPlayer(createPlayer("Merchant", "Merchant"));
    game.addPlayer(createPlayer("Other", "Spy"));

    for (int i = 0; i < 2; ++i) game.tax("Merchant"); // Have 6
    game.startTurn("Merchant");
    CHECK(game.getPlayerCoins("Merchant") >= 7); // Got 1 bonus
}
// Test case for Merchant role abilities, including gathering coins and receiving bonuses
TEST_CASE("General Role Abilities") {
    Game game;
    game.addPlayer(createPlayer("General", "General"));
    game.addPlayer(createPlayer("Enemy", "Spy"));

    for (int i = 0; i < 10; ++i) game.gather("Enemy");

    SUBCASE("General blocks coup") {
        game.coup("Enemy", "General");
        game.blockCoup("General", "General");
        CHECK(game.getActivePlayers().size() == 2);
    }
}

TEST_CASE("Judge Role Abilities") {
    Game game;
    game.addPlayer(createPlayer("Judge", "Judge"));
    game.addPlayer(createPlayer("Briber", "Baron"));

    for (int i = 0; i < 4; ++i) game.gather("Briber");

    game.bribe("Briber");
    game.blockBribe("Judge", "Briber");
    CHECK(game.getPlayerCoins("Briber") == 0);
}

TEST_CASE("Sanction and Arrest Mechanics") {
    Game game;
    game.addPlayer(createPlayer("Gov", "Governor"));
    game.addPlayer(createPlayer("Spy", "Spy"));

    for (int i = 0; i < 3; ++i) game.gather("Gov");

    game.sanction("Gov", "Spy");
    CHECK_THROWS(game.gather("Spy"));
    CHECK_THROWS(game.tax("Spy"));

    game.nextTurn();
    game.nextTurn();

    CHECK_NOTHROW(game.gather("Spy"));
}
// Test case for sanctioning a player and checking their actions
TEST_CASE("Game End and Winning") {
    Game game;
    game.addPlayer(createPlayer("A", "Governor"));
    game.addPlayer(createPlayer("B", "Spy"));

    for (int i = 0; i < 10; ++i) game.gather("A");
    game.coup("A", "B");
    CHECK(game.getWinner() == "A");
}
