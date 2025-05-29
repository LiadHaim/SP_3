//liadi.haim@gmail.com
#include "Game.hpp"
#include "GameGUI.hpp"

int main() {
    Game game;
    GameGUI gui(&game);
    gui.run();  // Start the GUI loop  
    return 0;
}
