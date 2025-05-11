# Poker
poker game

Current Structure:

Project Root
│
├── main.cpp # Entry point of the program
├── Makefile # For building the project
├── players.json # Stores persistent player data (e.g., scores)
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
├── empty.cpp # (Unused or placeholder file)
├── GameManager.cpp
├── Hand.cpp
├── Leaderboard.cpp
├── Player.cpp
├── Scorer.cpp
└── Shop.cpp
