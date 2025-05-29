// liadi.haim@gmail.com 
#pragma once
#include "Player.hpp"

class Judge : public Player {
public:
    Judge(const std::string& name);
    std::string getRole() const override;
    bool canBlockBribe() const override;
    void onSanctioned() override;
};