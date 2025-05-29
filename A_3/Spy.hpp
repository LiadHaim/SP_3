// liadi.haim@gmail.com 
#pragma once
#include "Player.hpp"

class Spy : public Player {
public:
    explicit Spy(const std::string& name);
    std::string getRole() const override;
    int spyOnPlayer(Player* target);
};
