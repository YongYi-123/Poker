# 🃏 Poker Game (Simplified Balatro Clone)

A command-line poker game implemented in C++ using object-oriented programming principles.  
Inspired by the roguelike deck-builder *Balatro*, this game allows players to log in, play poker rounds, and earn scores.


## 📂 Project Structure
<pre>
│
├── main.cpp # Entry point of the program
├── Makefile # For building the project
├── players.json # Stores persistent player data (e.g., scores, money, inventory)
├── README.md # Project documentation
│
├── inc/ # Header files (class declarations)
│ ├── Card.h
│ ├── Deck.h
│ ├── GameManager.h
│ ├── Hand.h
│ ├── Leaderboard.h
│ ├── Player.h
│ ├── Scorer.h
│ └── Shop.h
│
├── include/ # External dependencies
│ └── json.hpp # nlohmann/json library for JSON parsing
│
└── src/ # Source files (class implementations)
├── Card.cpp
├── Deck.cpp
├── GameManager.cpp
├── Hand.cpp
├── Leaderboard.cpp
├── Player.cpp
├── Scorer.cpp
└── Shop.cpp
</pre>

### Requirements
- C++11 or higher
- `g++` compiler
- `make`

### Build Instructions
```bash
make        # Builds the project
make run    # Builds and runs the game
make clean  # Cleans object and binary files
Output binary will be located in the bin/ directory as PokerGame.
