// liadi.haim@gmail.com 
#include "Merchant.hpp"

Merchant::Merchant(const std::string& name) : Player(name), receivedStartBonus(false) {}
// Constructor initializes the Merchant with a name and default values for coins and status
std::string Merchant::getRole() const { 
    return "Merchant"; 
}
// This function returns the role of the player as a string
void Merchant::onTurnStart() {
    receivedStartBonus = false;
    if (coins >= 3) {
        addCoins(1);
        receivedStartBonus = true;
    }
}

void Merchant::onArrested() {
    spendCoins(2); // משלם 2 מטבעות לקופה במקום לאבד אחד
}