// liadi.haim@gmail.com 
#include "Player.hpp"
#include <stdexcept>

Player::Player(const std::string& playerName) 
    : name(playerName), coins(2), isActive(true), sanctioned(false), cannotArrest(false) {}

const std::string& Player::getName() const { 
    return name; 
}

int Player::getCoins() const { 
    return coins; 
}
// This function returns the number of coins the player has
bool Player::getIsActive() const { 
    return isActive; 
}

bool Player::isSanctioned() const { 
    return sanctioned; 
}

bool Player::canArrest() const { 
    return !cannotArrest; 
}
// This function checks if the player can perform an arrest action
void Player::addCoins(int amount) { 
    if (amount < 0) throw std::invalid_argument("Cannot add negative coins");
    coins += amount; 
}

bool Player::spendCoins(int amount) {
    if (amount < 0) throw std::invalid_argument("Cannot spend negative coins");
    if (coins < amount) return false;
    coins -= amount;
    return true;
}
// This function attempts to spend a certain amount of coins and returns true if successful, false otherwise
void Player::eliminate() { 
    isActive = false; 
}

void Player::setSanctioned(bool sanctioned) { 
    this->sanctioned = sanctioned; 
}

void Player::setCannotArrest(bool cannot) { 
    this->cannotArrest = cannot; 
}

void Player::onTurnStart() {}

int Player::getTaxAmount() const { 
    return 2; 
}

bool Player::canBlockTax() const { 
    return false; 
}

bool Player::canBlockCoup(int cost) const { 
    return false; 
}

void Player::onSanctioned() {}

void Player::onArrested() {}

bool Player::canBlockBribe() const { 
    return false; 
}

void Player::gather() {
    if (sanctioned) throw std::runtime_error("Cannot gather while sanctioned");
    addCoins(1);
}
// This function allows the player to gather coins, increasing their total by 1
void Player::tax() {
    if (sanctioned) throw std::runtime_error("Cannot tax while sanctioned");
    addCoins(getTaxAmount());
}

bool Player::bribe() {
    if (!spendCoins(4)) return false;
    return true;
}

void Player::arrest(Player* target) {
    if (!canArrest()) throw std::runtime_error("Cannot arrest this turn");
    if (!target->isActive) throw std::invalid_argument("Cannot arrest inactive player");
    if (target->coins > 0) {
        target->coins--;
        addCoins(1);
    }
    target->onArrested();
}

void Player::sanction(Player* target) {
    if (!spendCoins(3)) throw std::runtime_error("Not enough coins for sanction");
    if (!target->isActive) throw std::invalid_argument("Cannot sanction inactive player");
    target->setSanctioned(true);
    target->onSanctioned();
}
// This function allows a player to sanction another player, making them unable to perform certain actions
void Player::coup(Player* target) {
    if (!spendCoins(7)) throw std::runtime_error("Not enough coins for coup");
    if (!target->isActive) throw std::invalid_argument("Cannot coup inactive player");
    target->eliminate();
}