
// liadi.haim@gmail.com 
#pragma once
#include "Player.hpp"

class Governor : public Player {
public:
    Governor(const std::string& name);
    std::string getRole() const override;
    int getTaxAmount() const override;
    bool canBlockTax() const override;
};