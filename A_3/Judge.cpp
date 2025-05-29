// liadi.haim@gmail.com 
#include "Judge.hpp"

Judge::Judge(const std::string& name) : Player(name) {}
// Constructor initializes the Judge with a name and default values for coins and status
std::string Judge::getRole() const { 
    return "Judge"; 
}
// This function returns the role of the player as a string
bool Judge::canBlockBribe() const { 
    return true; 
}
// This function returns true if the Judge can block a bribe action
void Judge::onSanctioned() {
  
}
