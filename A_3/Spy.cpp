// liadi.haim@gmail.com 
#include "Spy.hpp"
#include <stdexcept>

Spy::Spy(const std::string& name) : Player(name) {}
// Constructor initializes the Spy with a name and default values for coins and status
std::string Spy::getRole() const {
    return "Spy";
}
// This function returns the role of the player as a string
int Spy::spyOnPlayer(Player* target) {
    if (!target->getIsActive()) {
        throw std::invalid_argument("Cannot spy on inactive player");
    }
    target->setCannotArrest(true);
    return target->getCoins();
}
