//liadi.haim@gmail.com
#pragma once
#include "Player.hpp"
#include <vector>
#include <memory>
#include <string>

class Game {
private:
    std::vector<std::unique_ptr<Player>> players;  
    int currentPlayerIndex;
    int treasury;
    bool gameEnded;
    std::string winner;
// This class manages the game state, player actions, and game flow
public:
    Game();
    
    // Player management
    void addPlayer(std::unique_ptr<Player> player);
    
    // Game info functions
    std::string turn() const;
    std::vector<std::string> getActivePlayers() const;
    std::string getWinner();
    
    // Game flow
    void nextTurn();
    void checkGameEnd();
    
    // Player access
    Player& getCurrentPlayer();
    Player* findPlayer(const std::string& name);
    
    // Treasury functions
    void addToTreasury(int amount);
    bool takeFromTreasury(int amount);
    int getTreasuryAmount() const;
    
    // Display
    void printGameState() const;
    void gather(const std::string& name);
    void tax(const std::string& name);
    void coup(const std::string& source, const std::string& target);
    void arrest(const std::string& source, const std::string& target);
    void bribe(const std::string& name);
    void sanction(const std::string& source, const std::string& target);
    int getPlayerCoins(const std::string& name) const;
// Block actions
    void blockTax(const std::string& blocker, const std::string& target);
    void blockBribe(const std::string& blocker, const std::string& target);
    void blockCoup(const std::string& blocker, const std::string& target);
// Spy actions
    int spyAction(const std::string& spy, const std::string& target) const;
    void preventArrest(const std::string& spy, const std::string& target);
    void invest(const std::string& name);
    void startTurn(const std::string& name);

        

};
std::unique_ptr<Player> createPlayer(const std::string& name, const std::string& role);
