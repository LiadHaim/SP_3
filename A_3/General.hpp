//liadi.haim@gmail.com
#pragma once
#include "Player.hpp"

class General : public Player {
public://
    General(const std::string& name);
    std::string getRole() const override;
    bool canBlockCoup(int cost) const override;
    bool blockCoup();
    void onArrested() override;
};