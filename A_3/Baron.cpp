//liadi.haim@gmail.com
#include "Baron.hpp"

Baron::Baron(const std::string& name) : Player(name) {}
// Baron constructor initializes the player with a name
std::string Baron::getRole() const { 
    return "Baron"; 
}
// Returns the role of the player as a string
bool Baron::invest() {
    if (!spendCoins(3)) return false;
    addCoins(6);
    return true;
}
// Invests 3 coins to gain 6 coins, returns false if not enough coins
void Baron::onSanctioned() {
    addCoins(1); // מקבל פיצוי
}
