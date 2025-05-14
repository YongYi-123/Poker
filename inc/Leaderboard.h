#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>

class Leaderboard {
public:
    // Display top N players (default top 10)
    static void displayTopPlayers(int topN = 10, bool showHeader = true);

    // Update the JSON leaderboard with the latest score
    static void updatePlayerScore(const std::string& username, int score);

    static int getSavedScore(const std::string& username);

};

#endif
