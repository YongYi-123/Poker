#!/bin/bash

mkdir -p include src headers

# Create empty source files
touch src/{main.cpp,GameManager.cpp,Player.cpp,Deck.cpp,Hand.cpp,Card.cpp,Shop.cpp,Scorer.cpp,Leaderboard.cpp}

# Create empty header files
touch headers/{GameManager.h,Player.h,Deck.h,Hand.h,Card.h,Shop.h,Scorer.h,Leaderboard.h}

# Download JSON header
mkdir -p include
curl -o include/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp

# Create empty JSON save file
echo "{}" > players.json

# Create README and placeholder for report
touch README.md Student_ID_Report.pdf

echo "Project structure created!"
