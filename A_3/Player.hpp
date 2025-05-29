// liadi.haim@gmail.com 
#pragma once
#include <string>

class Player {
protected:
    std::string name;
    int coins;
    bool isActive;
    bool sanctioned;
    bool cannotArrest;
// This flag indicates if the player is sanctioned and cannot perform certain actions
public:
    Player(const std::string& playerName);
    virtual ~Player() = default;

    // Getters
    const std::string& getName() const;
    int getCoins() const;
    bool getIsActive() const;
    bool isSanctioned() const;
    bool canArrest() const;
    
    // Basic functions
    void addCoins(int amount);
    bool spendCoins(int amount);
    void eliminate();
    void setSanctioned(bool sanctioned);
    void setCannotArrest(bool cannot);
    
    // Virtual functions for special roles
    virtual std::string getRole() const = 0;
    virtual void onTurnStart();
    virtual int getTaxAmount() const;
    virtual bool canBlockTax() const;
    virtual bool canBlockCoup(int cost) const;
    virtual void onSanctioned();
    virtual void onArrested();
    virtual bool canBlockBribe() const;
    
    // Basic actions
    virtual void gather();
    virtual void tax();
    virtual bool bribe();
    virtual void arrest(Player* target);
    virtual void sanction(Player* target);
    virtual void coup(Player* target);
};
