#include "Leaderboard.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

using json = nlohmann::json;
const std::string FILE_NAME = "players.json";

void Leaderboard::displayTopPlayers(int topN) {
    std::ifstream inFile(FILE_NAME);
    if (!inFile) {
        std::cerr << "Unable to open leaderboard file.\n";
        return;
    }

    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string content = buffer.str();
    inFile.close();

    if (content.empty()) {
        std::cout << "No leaderboard data found.\n";
        return;
    }

    json players = json::parse(content, nullptr, false);
    if (players.is_discarded()) {
        std::cerr << "Invalid JSON in leaderboard file.\n";
        return;
    }

    std::vector<std::pair<std::string, int>> scoreList;

    for (auto& [username, info] : players.items()) {
        int score = 0;
        if (info.is_object()) {
            score = info.value("score", 0);  // ✅ Safe access
        }
        scoreList.emplace_back(username, score);
    }

    std::sort(scoreList.begin(), scoreList.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second; // descending
              });

    std::cout << "====== Leaderboard ======\n";
    for (int i = 0; i < std::min(topN, static_cast<int>(scoreList.size())); ++i) {
        std::cout << i + 1 << ". " << scoreList[i].first
                  << " - " << scoreList[i].second << " pts\n";
    }
}

void Leaderboard::updatePlayerScore(const std::string& username, int score) {
    std::ifstream inFile(FILE_NAME);
    json players;

    if (inFile.is_open()) {
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        std::string content = buffer.str();
        inFile.close();

        if (!content.empty()) {
            players = json::parse(content, nullptr, false);
            if (players.is_discarded()) {
                std::cerr << "Warning: players.json is invalid. Reinitializing.\n";
                players = json::object();
            }
        }
    }

    int oldScore = 0;
    if (players.contains(username) && players[username].is_object()) {
        oldScore = players[username].value("score", 0);  // ✅ Safe access
    }

    if (score > oldScore) {
        players[username]["score"] = score;
    }

    std::ofstream outFile(FILE_NAME);
    if (!outFile) {
        std::cerr << "Unable to write to leaderboard file.\n";
        return;
    }

    outFile << players.dump(4);  // pretty print
    outFile.close();
}
int Leaderboard::getSavedScore(const std::string& username) {
    std::ifstream in("players.json");
    if (!in.is_open()) return 0;

    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string content = buffer.str();
    in.close();

    if (content.empty()) return 0;

    json data = json::parse(content, nullptr, false);
    if (data.is_discarded()) return 0;

    if (data.contains(username) && data[username].is_object()) {
        return data[username].value("score", 0);
    }

    return 0;
}
