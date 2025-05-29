// liadi.haim@gmail.com 
#include "Governor.hpp"

Governor::Governor(const std::string& name) : Player(name) {}
// Constructor initializes the Governor with a name and default values for coins and status
std::string Governor::getRole() const { 
    return "Governor"; 
}
// This function returns the role of the player as a string
int Governor::getTaxAmount() const { 
    return 3; 
}

bool Governor::canBlockTax() const { 
    return true; 
}