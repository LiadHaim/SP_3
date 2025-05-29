//liadi.haim@gmail.com
#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

class Game;


class GameGUI {
public:
    // Constructor and Destructor
    explicit GameGUI(Game* gamePtr);
    ~GameGUI();

    // Main game loop methods
    void run();
    void update();
    void render();
    void refresh();

    // Window management
    bool isOpen() const;
    void close();

    // Message display methods
    void showMessage(const std::string& message);
    void showError(const std::string& message);
    void showSuccess(const std::string& message);
    void clearMessage();

private:
    // Window constants
    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 700;
    static constexpr int BUTTON_WIDTH = 120;
    static constexpr int BUTTON_HEIGHT = 35;

    // UI States
    enum class UIState {
        ACTION_SELECTION,
        TARGET_SELECTION,
        CONFIRMATION,
        GAME_OVER
    };

    // Button structure
    struct Button {
        sf::RectangleShape shape;
        sf::Text text;
        std::string action;
        bool enabled;

        Button() : enabled(true) {}
    };

    // Core game data
    Game* game;
    bool gameRunning;
    bool gameFinished;
    UIState currentState;

    // SFML objects
    sf::RenderWindow window;
    sf::Font font;

    // UI elements
    sf::Text titleText;
    sf::Text turnText;
    sf::Text coinsText;
    sf::Text statusText;
    sf::Text instructionsText;

    // Buttons
    std::vector<Button> actionButtons;
    std::vector<Button> playerButtons;
    Button confirmButton;
    Button cancelButton;

    // Colors
    sf::Color backgroundColor;
    sf::Color buttonColor;
    sf::Color buttonHoverColor;
    sf::Color buttonDisabledColor;
    sf::Color textColor;
    sf::Color highlightColor;

    // Game state
    std::string selectedAction;
    std::string targetPlayer;
    std::string statusMessage;

    // Initialization methods
    void initializeWindow();
    void initializeUI();
    void setupColors();
    void setupButtons();
    bool loadFont();

    // Button management
    void createButton(Button& button, const std::string& text, float x, float y, const std::string& action);
    bool isButtonClicked(const Button& button, const sf::Vector2i& mousePos) const;
    void updateButtonAppearance(Button& button, bool isHovered);
    void updateButtonStates();

    // Event handling
    void handleEvents();
    void handleMouseClick(const sf::Vector2i& mousePos);
    void handleKeyPress(sf::Keyboard::Key key);

    // Rendering methods
    void renderBackground();
    void renderTitle();
    void renderGameState();
    void renderPlayers();
    void renderActionButtons();
    void renderPlayerButtons();
    void renderConfirmation();
    void renderStatusMessage();
    void renderGameOver();

    // Game logic helpers
    std::string getPlayerRole(const std::string& playerName) const;
    int getPlayerCoins(const std::string& playerName) const;
    std::vector<std::string> getActivePlayers() const;
    std::string getCurrentPlayer() const;
    bool canPerformAction(const std::string& action) const;
    bool needsTarget(const std::string& action) const;

    // Action execution
    void executeAction();
    void resetSelection();

    // UI state management
    void updateStatusMessage();

    // Prevent copy construction and assignment
    GameGUI(const GameGUI&) = delete;
    GameGUI& operator=(const GameGUI&) = delete;
};

#endif 