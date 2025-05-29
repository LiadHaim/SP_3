//liadi.haim@gmail.com
#pragma once
#include "Player.hpp"

class Baron : public Player { 
public:
    Baron(const std::string& name);// Constructor
    std::string getRole() const override;
    bool invest();
    void onSanctioned() override;
};