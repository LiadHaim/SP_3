//liadi.haim@gmail.com
#include "Game.hpp"
#include <stdexcept>
#include <iostream>
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
//
Game::Game() : currentPlayerIndex(0), treasury(50), gameEnded(false) {}
// Factory function to create players based on their role
std::unique_ptr<Player> createPlayer(const std::string& name, const std::string& role) {
    if (role == "Governor") return std::make_unique<Governor>(name);
    if (role == "Spy") return std::make_unique<Spy>(name);
    if (role == "Baron") return std::make_unique<Baron>(name);
    if (role == "General") return std::make_unique<General>(name);
    if (role == "Judge") return std::make_unique<Judge>(name);
    if (role == "Merchant") return std::make_unique<Merchant>(name);
    throw std::invalid_argument("Unknown role: " + role);
}
// Add a player to the game
void Game::addPlayer(std::unique_ptr<Player> player) {
    if (gameEnded) throw std::runtime_error("Cannot add players to ended game");
    players.push_back(std::move(player));
}
// Gather coins for a player`
std::string Game::turn() const {
    if (players.empty()) return "No players";

    if (gameEnded) return winner + " (Game Ended)";
    if (players.empty()) return "No players"; // תיקון מונע קריסה
    return players[currentPlayerIndex]->getName();
}

std::vector<std::string> Game::getActivePlayers() const {
    std::vector<std::string> activePlayers;
    for (const auto& player : players) {
        if (player->getIsActive()) {
            activePlayers.push_back(player->getName());
        }
    }
    return activePlayers;
}

std::string Game::getWinner() {
    if (!gameEnded) throw std::runtime_error("Game is still active");
    return winner;
}

void Game::nextTurn() {
    if (gameEnded) return;

    for (auto& player : players) {
        player->setSanctioned(false);
        player->setCannotArrest(false);
    }

    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (!players[currentPlayerIndex]->getIsActive());

    players[currentPlayerIndex]->onTurnStart();

    if (players[currentPlayerIndex]->getCoins() >= 10) {
        std::cout << players[currentPlayerIndex]->getName()
                  << " must perform a coup (has 10+ coins)" << std::endl;
    }

    checkGameEnd();
}

void Game::checkGameEnd() {
    auto activePlayers = getActivePlayers();
    if (activePlayers.size() == 1) {
        gameEnded = true;
        winner = activePlayers[0];
    }
}

Player& Game::getCurrentPlayer() {
    if (gameEnded) throw std::runtime_error("Game has ended");
    return *players[currentPlayerIndex];
}

Player* Game::findPlayer(const std::string& name) {
    for (auto& player : players) {
        if (player->getName() == name && player->getIsActive()) {
            return player.get();
        }
    }
    return nullptr;
}

void Game::addToTreasury(int amount) {
    treasury += amount;
}

bool Game::takeFromTreasury(int amount) {
    if (treasury < amount) return false;
    treasury -= amount;
    return true;
}

int Game::getTreasuryAmount() const {
    return treasury;
}
// Gather coins for a player
void Game::printGameState() const {
    std::cout << "\n=== Game State ===" << std::endl;
    std::cout << "Treasury: " << treasury << " coins" << std::endl;
    std::cout << "Current turn: " << turn() << std::endl;
    std::cout << "\nPlayers:" << std::endl;

    for (const auto& player : players) {
        if (player->getIsActive()) {
            std::cout << "- " << player->getName()
                      << " (" << player->getRole() << "): "
                      << player->getCoins() << " coins";
            if (player->isSanctioned()) std::cout << " [SANCTIONED]";
            if (!player->canArrest()) std::cout << " [CANNOT ARREST]";
            std::cout << std::endl;
        }
    }
    std::cout << "==================\n" << std::endl;
} 
