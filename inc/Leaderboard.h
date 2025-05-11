#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <utility> // for std::pair

class Leaderboard {
public:
    // Display top N players (sorted by score)
    static void displayTopPlayers(int topN = 10);

    // Update the JSON leaderboard with the latest score
    static void updatePlayerScore(const std::string& username, int score);
};

#endif
 