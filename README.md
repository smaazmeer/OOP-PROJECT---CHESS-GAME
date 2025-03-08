# OOP-PROJECT---CHESS-GAME

## Overview
This is a GUI-based Chess game built in C++ featuring:
- **Player vs Player (2-Player Mode)**
- **Player vs Computer (AI Mode)** using the Stockfish chess engine.
- **Pre-configured AI Bots** with different difficulty levels, each having a set Stockfish thinking depth.

## Features
- **Graphical User Interface (GUI)** for an interactive chess experience.
- **Stockfish Integration** for AI-based opponent moves.
- **Multiple AI Bots** with predefined difficulty settings.
- **Smooth and Responsive Gameplay** with an intuitive interface.
- **Legal Move Indication** to assist players during the game.
- **Move History and Game Notation** for tracking gameplay.

## Installation
### Prerequisites
Ensure you have the following installed:
- **C++ Compiler** (GCC, Clang, or MSVC)
- **SFML Library** for GUI rendering
- **Stockfish Engine** (precompiled binary or source code)

### Setup Instructions
1. **Clone the Repository**:
   ```sh
   git clone https://github.com/smaazmeer/chess-game.git
   cd chess-game
   ```

2. **Install SFML** (if not installed):
   - On Windows (Using vcpkg):
     ```sh
     vcpkg install sfml
     ```

   - On macOS Installation Instructions:

      1. **Install Homebrew** (if not installed):  
         ```sh
         /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
         ```
      
      2. **Install SFML**:  
         ```sh
         brew install sfml
         ```

3. **Download Stockfish** from [Stockfish official site](https://stockfishchess.org/download/) and place it in the project directory.

4. **Compile the Game**:  
   ```sh
   g++ main.cpp -o chess-game -lsfml-graphics -lsfml-window -lsfml-system
   ```

5. **Run the Game**:  
   ```sh
   ./chess-game
   ```



3. **Download Stockfish**:
   - Get the latest Stockfish binary from [official site](https://stockfishchess.org/download/).
   - Place it in the project directory.

4. **Compile the Game**:
   ```sh
   g++ main.cpp -o chess-game -lsfml-graphics -lsfml-window -lsfml-system
   ```

5. **Run the Game**:
   ```sh
   ./chess-game
   ```

## How to Play
1. **Select Game Mode**:
   - Player vs Player
   - Player vs Computer
2. **Choose AI Bot** (if playing against the computer):
   - Each bot has a pre-configured Stockfish thinking level.
3. **Make Your Moves**:
   - Click on a piece to see legal moves.
   - Click on a destination square to move the piece.
4. **Win by Checkmate!**

## Collaborators
- **k241008@nu.edu.pk**
- **k240822@nu.edu.pk**

## Acknowledgments
- **SFML** for GUI rendering
- **Stockfish** for AI-based chess gameplay
