#include "Leaderboard.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
const std::string FILE_NAME = "players.json";

void Leaderboard::displayTopPlayers(int topN) {
    std::ifstream inFile(FILE_NAME);
    if (!inFile) {
        std::cerr << "Unable to open leaderboard file.\n";
        return;
    }

    json players;
    inFile >> players;
    inFile.close();

    std::vector<std::pair<std::string, int>> scoreList;

    for (auto& [username, info] : players.items()) {
        int score = info.value("score", 0);
        scoreList.emplace_back(username, score);
    }

    std::sort(scoreList.begin(), scoreList.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second; // descending
              });

    std::cout << "====== Leaderboard ======\n";
    for (int i = 0; i < std::min(topN, (int)scoreList.size()); ++i) {
        std::cout << i + 1 << ". " << scoreList[i].first
                  << " - " << scoreList[i].second << " pts\n";
    }
}

void Leaderboard::updatePlayerScore(const std::string& username, int score) {
    std::ifstream inFile(FILE_NAME);
    json players;

    if (inFile.is_open()) {
        inFile >> players;
        inFile.close();
    }

    int oldScore = players[username].value("score", 0);
    if (score > oldScore) {
        players[username]["score"] = score;
    }

    std::ofstream outFile(FILE_NAME);
    if (!outFile) {
        std::cerr << "Unable to write to leaderboard file.\n";
        return;
    }

    outFile << players.dump(4);
    outFile.close();
}
