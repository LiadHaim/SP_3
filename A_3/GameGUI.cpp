//liadi.haim@gmail.com
#include "GameGUI.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>

GameGUI::GameGUI(Game* gamePtr) 
    : game(gamePtr), gameRunning(true), gameFinished(false), 
      currentState(UIState::ACTION_SELECTION) {
    
    if (!game) {
        throw std::invalid_argument("Game pointer cannot be null");
    }
    
    initializeWindow();
    initializeUI();
}
// Destructor to clean up resources
GameGUI::~GameGUI() {
    if (window.isOpen()) {
        window.close();
    }
}
// Initialize the main game window
void GameGUI::initializeWindow() {
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
                  L"Coup -", 
                  sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    
    setupColors();
}
// Set up the default colors for the GUI
void GameGUI::setupColors() {
    backgroundColor = sf::Color(45, 45, 55);
    buttonColor = sf::Color(70, 130, 180);
    buttonHoverColor = sf::Color(100, 149, 237);
    buttonDisabledColor = sf::Color(100, 100, 100);
    textColor = sf::Color::White;
    highlightColor = sf::Color(255, 215, 0);
}
// Initialize the UI components
void GameGUI::initializeUI() {
        if (!loadFont()) {
            std::cerr << "Fatal: Could not load font. Exiting...\n";
            exit(1);
        }
    
        // Initialize title
        titleText.setFont(font);
        titleText.setString(L"Coup - Game");
        titleText.setCharacterSize(36);
        titleText.setFillColor(highlightColor);
        titleText.setPosition(WINDOW_WIDTH / 2 - 150, 20);
    
        // Initialize turn text
        turnText.setFont(font);
        turnText.setCharacterSize(24);
        turnText.setFillColor(textColor);
        turnText.setPosition(50, 80);
    
        // Initialize coins text
        coinsText.setFont(font);
        coinsText.setCharacterSize(18);
        coinsText.setFillColor(textColor);
        coinsText.setPosition(50, 120);
    
        // Initialize status text
        statusText.setFont(font);
        statusText.setCharacterSize(20);
        statusText.setFillColor(sf::Color::Yellow);
        statusText.setPosition(50, WINDOW_HEIGHT - 100);
    
        // Initialize instructions
        instructionsText.setFont(font);
        instructionsText.setString("Select an action to perform:");
        instructionsText.setCharacterSize(18);
        instructionsText.setFillColor(textColor);
        instructionsText.setPosition(50, 160);
    
        setupButtons();
    
    
}
// Load the font from a file
bool GameGUI::loadFont() {
    std::string path = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    if (!font.loadFromFile(path)) {
        std::cerr << "ERROR: Failed to load font from: " << path << std::endl;
        std::cerr << "Try running: sudo apt install fonts-dejavu\n";
        return false;
    }
    std::cout << "Font loaded successfully from: " << path << std::endl;
    return true;
}

// Set up the action and player buttons

void GameGUI::setupButtons() {
    actionButtons.clear();
    playerButtons.clear();
    
    // Action buttons
    std::vector<std::pair<std::string, std::string>> actions = {
        {"Gather", "gather"},
        {"Tax", "tax"},
        {"Bribe", "bribe"},
        {"Arrest", "arrest"},
        {"Sanction", "sanction"},
        {"Coup", "coup"}
    };
    
    int y = 200;
    for (const auto& action : actions) {
        Button button;
        createButton(button, action.first, 50, y, action.second);
        actionButtons.push_back(button);
        y += BUTTON_HEIGHT + 10;
    }
    
    // Confirm and Cancel buttons
    createButton(confirmButton, "Confirm", WINDOW_WIDTH - 200, WINDOW_HEIGHT - 150, "confirm");
    createButton(cancelButton, "Cancel", WINDOW_WIDTH - 200, WINDOW_HEIGHT - 100, "cancel");
    
    confirmButton.shape.setFillColor(sf::Color::Green);
    cancelButton.shape.setFillColor(sf::Color::Red);
}

void GameGUI::createButton(Button& button, const std::string& text, float x, float y, const std::string& action) {
    button.shape.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    button.shape.setPosition(x, y);
    button.shape.setFillColor(buttonColor);
    button.shape.setOutlineThickness(2);
    button.shape.setOutlineColor(sf::Color::White);
    
    button.text.setFont(font);
    button.text.setString(text);
    button.text.setCharacterSize(16);
    button.text.setFillColor(textColor);
    
    // Center text in button
    sf::FloatRect textBounds = button.text.getLocalBounds();
    button.text.setPosition(
        x + (BUTTON_WIDTH - textBounds.width) / 2,
        y + (BUTTON_HEIGHT - textBounds.height) / 2 - 2
    );
    
    button.action = action;
    button.enabled = true;
}

void GameGUI::run() {
    while (window.isOpen() && gameRunning) {
        handleEvents();
        update();
        render();
    }
}
// Handle user input events
void GameGUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                gameRunning = false;
                break;
                
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handleMouseClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                break;
                
            case sf::Event::KeyPressed:
                handleKeyPress(event.key.code);
                break;
                
            default:
                break;
        }
    }
}
// Handle mouse click events
void GameGUI::handleMouseClick(const sf::Vector2i& mousePos) {
    switch (currentState) {
        case UIState::ACTION_SELECTION:
            // Check action buttons
            for (const auto& button : actionButtons) {
                if (isButtonClicked(button, mousePos) && button.enabled) {
                    selectedAction = button.action;
                    if (needsTarget(selectedAction)) {
                        currentState = UIState::TARGET_SELECTION;
                        showMessage("Select target player");
                    } else {
                        currentState = UIState::CONFIRMATION;
                        showMessage("Confirm action: " + selectedAction);
                    }
                    break;
                }
            }
            break;
            
        case UIState::TARGET_SELECTION:
            // Check player buttons
            for (const auto& button : playerButtons) {
                if (isButtonClicked(button, mousePos) && button.enabled) {
                    targetPlayer = button.action; // action field stores player name
                    currentState = UIState::CONFIRMATION;
                    showMessage("Confirm action: " + selectedAction + " on " + targetPlayer);
                    break;
                }
            }
            
            // Check cancel button
            if (isButtonClicked(cancelButton, mousePos)) {
                resetSelection();
            }
            break;
            
        case UIState::CONFIRMATION:
            if (isButtonClicked(confirmButton, mousePos)) {
                executeAction();
            } else if (isButtonClicked(cancelButton, mousePos)) {
                resetSelection();
            }
            break;
            
        default:
            break;
    }
}
// Handle key press events
void GameGUI::handleKeyPress(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Escape:
            resetSelection();
            break;
            
        case sf::Keyboard::Enter:
            if (currentState == UIState::CONFIRMATION) {
                executeAction();
            }
            break;
            
        default:
            break;
    }
}
// Show a message in the status area
void GameGUI::update() {
    updateButtonStates();
    updateStatusMessage();
    
    // Check if game is finished
    try {
        std::string winner = game->getWinner();
        if (!winner.empty()) {
            gameFinished = true;
            currentState = UIState::GAME_OVER;
            showMessage("Game Over! Winner: " + winner);
        }
    } catch (...) {
        // Game is still active
    }
    
    // Update player buttons for target selection
    if (currentState == UIState::TARGET_SELECTION) {
        playerButtons.clear();
        auto players = getActivePlayers();
        std::string currentPlayer = getCurrentPlayer();
        
        int y = 500;
        for (const auto& player : players) {
            if (player != currentPlayer || selectedAction == "coup") { // Can coup self in some cases
                Button button;
                createButton(button, player, 300, y, player);
                playerButtons.push_back(button);
                y += BUTTON_HEIGHT + 10;
            }
        }
    }
}
// Execute the selected action
void GameGUI::render() {
    window.clear(backgroundColor);
    
    renderBackground();
    renderTitle();
    renderGameState();
    renderPlayers();
    
    switch (currentState) {
        case UIState::ACTION_SELECTION:
            renderActionButtons();
            break;
            
        case UIState::TARGET_SELECTION:
            renderActionButtons();
            renderPlayerButtons();
            renderConfirmation();
            break;
            
        case UIState::CONFIRMATION:
            renderActionButtons();
            renderConfirmation();
            break;
            
        case UIState::GAME_OVER:
            renderGameOver();
            break;
    }
    
    renderStatusMessage();
    
    window.display();
}
// Check if the game window is open
void GameGUI::renderBackground() {
    // Simple gradient background
    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setFillColor(backgroundColor);
    window.draw(bg);
}

void GameGUI::renderTitle() {
    window.draw(titleText);
}
// Render the current game state information
void GameGUI::renderGameState() {
    // Update turn information
    std::string currentPlayer = getCurrentPlayer();
    turnText.setString("Current Turn: " + currentPlayer);
    window.draw(turnText);
    
    // Update coins information
    std::stringstream coinsStream;
    coinsStream << "Treasury: " << /* game->getTreasury() */ "∞ coins";
    coinsText.setString(coinsStream.str());
    window.draw(coinsText);
    
    window.draw(instructionsText);
}

void GameGUI::renderPlayers() {
    auto players = getActivePlayers();
    std::string currentPlayer = getCurrentPlayer();
    
    int startY = 200;
    int x = 600;
    
    sf::Text playerText;
    playerText.setFont(font);
    playerText.setCharacterSize(16);
    
    for (size_t i = 0; i < players.size(); ++i) {
        const std::string& player = players[i];
        
        // Highlight current player
        if (player == currentPlayer) {
            playerText.setFillColor(highlightColor);
        } else {
            playerText.setFillColor(textColor);
        }
        
        std::stringstream playerInfo;
        playerInfo << player << " (" << getPlayerRole(player) << ") - " 
                  << getPlayerCoins(player) << " coins";
        
        playerText.setString(playerInfo.str());
        playerText.setPosition(x, startY + i * 25);
        window.draw(playerText);
    }
}

void GameGUI::renderActionButtons() {
    for (const auto& button : actionButtons) {
        sf::Color buttonColor = button.enabled ? 
            (selectedAction == button.action ? buttonHoverColor : this->buttonColor) : 
            buttonDisabledColor;
        
        sf::RectangleShape shape = button.shape;
        shape.setFillColor(buttonColor);
        window.draw(shape);
        window.draw(button.text);
    }
}
//  Render the player buttons for target selection
void GameGUI::renderPlayerButtons() {
    for (const auto& button : playerButtons) {
        sf::Color buttonColor = button.enabled ? 
            (targetPlayer == button.action ? buttonHoverColor : this->buttonColor) : 
            buttonDisabledColor;
        
        sf::RectangleShape shape = button.shape;
        shape.setFillColor(buttonColor);
        window.draw(shape);
        window.draw(button.text);
    }
}
//  Render the confirmation buttons for action execution
void GameGUI::renderConfirmation() {
    if (currentState == UIState::CONFIRMATION || currentState == UIState::TARGET_SELECTION) {
        window.draw(confirmButton.shape);
        window.draw(confirmButton.text);
    }
    
    window.draw(cancelButton.shape);
    window.draw(cancelButton.text);
}

void GameGUI::renderStatusMessage() {
    if (!statusMessage.empty()) {
        statusText.setString(statusMessage);
        window.draw(statusText);
    }
}

void GameGUI::renderGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);
    
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER!");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 100);
    window.draw(gameOverText);
    
    try {
        sf::Text winnerText;
        winnerText.setFont(font);
        winnerText.setString("Winner: " + game->getWinner());
        winnerText.setCharacterSize(32);
        winnerText.setFillColor(highlightColor);
        winnerText.setPosition(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50);
        window.draw(winnerText);
    } catch (...) {
        // Ignore if winner() throws
    }
}

void GameGUI::updateButtonStates() {
    std::string currentPlayer = getCurrentPlayer();
    
    for (auto& button : actionButtons) {
        button.enabled = canPerformAction(button.action);
    }
}

void GameGUI::updateStatusMessage() {
    // Status message is updated by other methods as needed
}

void GameGUI::resetSelection() {
    selectedAction.clear();
    targetPlayer.clear();
    currentState = UIState::ACTION_SELECTION;
    clearMessage();
}

bool GameGUI::isButtonClicked(const Button& button, const sf::Vector2i& mousePos) const {
    sf::FloatRect bounds = button.shape.getGlobalBounds();
    return bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}
// Update the appearance of a button based on its state
void GameGUI::updateButtonAppearance(Button& button, bool isHovered) {
    if (isHovered && button.enabled) {
        button.shape.setFillColor(buttonHoverColor);
    } else if (button.enabled) {
        button.shape.setFillColor(buttonColor);
    } else {
        button.shape.setFillColor(buttonDisabledColor);
    }
}

std::string GameGUI::getPlayerRole(const std::string& playerName) const {
    (void)playerName;
    try {
        
        return "Unknown"; // Placeholder
    } catch (...) {
        return "Unknown";
    }
}

int GameGUI::getPlayerCoins(const std::string& playerName) const {
    (void)playerName;

    try {

        return 0; // Placeholder
    } catch (...) {
        return 0;
    }
}

std::vector<std::string> GameGUI::getActivePlayers() const {
    try {
        return game->getActivePlayers();
    } catch (...) {
        return {};
    }
}

std::string GameGUI::getCurrentPlayer() const {
    try {
        return game->turn();
    } catch (...) {
        return "Unknown";
    }
}

bool GameGUI::canPerformAction(const std::string& action) const {

    std::string currentPlayer = getCurrentPlayer();
    int coins = getPlayerCoins(currentPlayer);
    
    if (action == "gather") return true;
    if (action == "tax") return true;
    if (action == "bribe") return coins >= 4;
    if (action == "arrest") return true;
    if (action == "sanction") return coins >= 3;
    if (action == "coup") return coins >= 7;
    
    return false;
}

bool GameGUI::needsTarget(const std::string& action) const {
    return action == "arrest" || action == "sanction" || action == "coup";
}

void GameGUI::executeAction() {
    try {
        
        if (selectedAction == "gather") {
            // game->gather(getCurrentPlayer());
        } else if (selectedAction == "tax") {
            // game->tax(getCurrentPlayer());
        } else if (selectedAction == "bribe") {
            // game->bribe(getCurrentPlayer());
        } else if (selectedAction == "arrest") {
            // game->arrest(getCurrentPlayer(), targetPlayer);
        } else if (selectedAction == "sanction") {
            // game->sanction(getCurrentPlayer(), targetPlayer);
        } else if (selectedAction == "coup") {
            // game->coup(getCurrentPlayer(), targetPlayer);
        }
        
        showSuccess("Action executed successfully!");
        resetSelection();
        
    } catch (const std::exception& e) {
        showError("Error: " + std::string(e.what()));
        resetSelection();
    }
}

void GameGUI::showMessage(const std::string& message) {
    statusMessage = message;
}

void GameGUI::clearMessage() {
    statusMessage.clear();
}

bool GameGUI::isOpen() const {
    return window.isOpen();
}

void GameGUI::close() {
    window.close();
    gameRunning = false;
}

void GameGUI::refresh() {
    update();
}

void GameGUI::showError(const std::string& message) {
    statusText.setFillColor(sf::Color::Red);
    showMessage("ERROR: " + message);
}

void GameGUI::showSuccess(const std::string& message) {
    statusText.setFillColor(sf::Color::Green);
    showMessage("SUCCESS: " + message);
}