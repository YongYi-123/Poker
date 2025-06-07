# 🃏 Poker Game (Simplified Balatro-Inspired)

This is a simplified roguelike deck-building poker game implemented in C++ using object-oriented programming (OOP) principles.  
It’s inspired by *Balatro*, combining traditional poker scoring with strategic deckplay and optional item mechanics.

---

## 📁 Folder Structure
<pre>
│
├── main.cpp              # Entry point of the program  
├── Makefile              # For building the project  
├── players.json          # Stores persistent player data (e.g., scores, money, inventory)  
├── README.md             # Project documentation  
│
├── inc/                  # Header files (class declarations)  
│   ├── Card.h  
│   ├── Deck.h  
│   ├── GameManager.h  
│   ├── Hand.h  
│   ├── Leaderboard.h  
│   ├── Player.h  
│   ├── Scorer.h  
│   └── Shop.h  
│
├── include/              # External dependencies  
│   └── json.hpp          # nlohmann/json library for JSON parsing  
│
└── src/                  # Source files (class implementations)  
    ├── Card.cpp  
    ├── Deck.cpp  
    ├── GameManager.cpp  
    ├── Hand.cpp  
    ├── Leaderboard.cpp  
    ├── Player.cpp  
    ├── Scorer.cpp  
    └── Shop.cpp
</pre>


---

## 🚀 Features

### ✅ Core Gameplay
- Draw 8 cards per game session
- 4 rounds of card playing (1–5 cards each)
- 3 rounds of discards (1–5 cards each)
- Hand sorting: by value or suit
- Hand type detection (e.g. Flush, Pair, Full House)
- Score calculation using multipliers

### 💰 Bonus Features
- In-game currency: earn money from score
- Shop: purchase tickets with unique effects
- Inventory: store and use purchased items

### 🏆 Player System
- Login and logout
- Persistent scores and inventory stored in `players.json`
- Leaderboard showing top player scores

---

## 🔧 Build Instructions

### Requirements
- `g++` with C++17 or higher
- Make (optional but recommended)
- Terminal (Windows users: Git Bash or WSL preferred)

### Commands

```bash
make         # Compile all files
make run     # Compile and run the game
make clean   # Remove compiled binaries and object files
