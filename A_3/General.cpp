//liadi.haim@gmail.com
#include "General.hpp"

General::General(const std::string& name) : Player(name) {}
// Constructor initializes the General with a name and default values for coins and status
std::string General::getRole() const { 
    return "General"; 
}

bool General::canBlockCoup(int cost) const { 
    return coins >= 5; // General can block a coup if they have at least 5 coins
}

bool General::blockCoup() {// This function is called when a coup is attempted against the General
    return spendCoins(5);
}

void General::onArrested() {
    addCoins(1); // General gets 1 coin when arrested
}
