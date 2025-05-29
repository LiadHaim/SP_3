// liadi.haim@gmail.com 
#pragma once
#include "Player.hpp"

class Merchant : public Player {
private:
    bool receivedStartBonus;
// This flag indicates if the Merchant has received their start bonus for the turn
public:
    Merchant(const std::string& name);
    std::string getRole() const override;
    void onTurnStart() override;
    void onArrested() override;
};