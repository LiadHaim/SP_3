# SP_3
# Coup Game

A strategic card game with SFML-based GUI, inspired by the popular Coup card game.

## Requirements

- **C++17** or newer
- **SFML 2.5+**

### Install SFML
```bash
# Ubuntu/Debian
sudo apt-get install libsfml-dev

# Windows (vcpkg)
vcpkg install sfml

# macOS (Homebrew)
brew install sfml
```

## Build & Run

```bash
# Using CMake (recommended)
mkdir build && cd build
cmake ..
make
./coup-game

# Or direct compilation
g++ -std=c++17 src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system -o coup-game
```

## Project Structure

```
src/
├── gameGui.cpp/hpp    # GUI implementation
├── Game.cpp/hpp       # Game logic
├── Player.cpp/hpp     # Player management
└── main.cpp           # Entry point
```

## Game Actions

- **Gather** - Take 1 coin
- **Tax** - Take 3 coins (role required)
- **Bribe** - Pay 4 coins, get 2 coins
- **Arrest** - Block player's influence
- **Sanction** - Make player lose coins (costs 3)
- **Coup** - Eliminate player (costs 7)

## Controls

- **Mouse** - Click buttons and select actions
- **ESC** - Cancel current action
- **Enter** - Confirm action

## Features

- Hebrew and English support
- Interactive GUI with hover effects
- Dynamic button states
- Multi-player support
- Real-time game state updates

## Troubleshooting

**Font issues**: Place `arial.ttf` in `assets/fonts/` directory
**SFML not found**: Verify installation with `pkg-config --modversion sfml-all`
**Build errors**: Ensure C++17 compiler support

## License

MIT License
