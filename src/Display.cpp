#include "Display.h"
#include "Hand.h"
#include "Leaderboard.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

void drawHandOnlyInterface(const Player& player,
                           const vector<Card>& handCards,
                           int playRoundsLeft,
                           int discardRoundsLeft) {
    cout << "\033[H\033[J";  // Clear screen

    cout << right << setw(70) << "SCORE    : " << player.getScore() << "\n";
    cout << right << setw(70) << "HANDS    : " << playRoundsLeft << "\n";
    cout << right << setw(70) << "DISCARDS : " << discardRoundsLeft << "\n\n";

    cout << "╔════════════════════════════════════════════════════════════════════╗\n";

    cout << "║   ";
    for (size_t i = 0; i < handCards.size(); ++i)
        cout << "  " << setw(2) << i << "    ";
    cout << string((8 - handCards.size()) * 8, ' ') << " ║\n";

    cout << "║   ";
    for (size_t i = 0; i < handCards.size(); ++i)
        cout << "┌─────┐ ";
    cout << string((8 - handCards.size()) * 8, ' ') << " ║\n";

    cout << "║   ";
    for (const Card& card : handCards)
        cout << "│ " << setw(2) << card.getFaceStr() << "  │ ";
    cout << string((8 - handCards.size()) * 8, ' ') << " ║\n";

    cout << "║   ";
    for (const Card& card : handCards)
        cout << "│  " << card.getSuitEmoji() << "  │ ";
    cout << string((8 - handCards.size()) * 8, ' ') << " ║\n";

    cout << "║   ";
    for (size_t i = 0; i < handCards.size(); ++i)
        cout << "└─────┘ ";
    cout << string((8 - handCards.size()) * 8, ' ') << " ║\n";

    cout << "╚════════════════════════════════════════════════════════════════════╝\n";
}

void drawResultInterface(const Player& player,
                         const vector<Card>& played,
                         const vector<Card>& remaining,
                         const vector<int>& playedIndices,
                         int playRoundsLeft,
                         int discardRoundsLeft,
                         const string& handType,
                         int handScore,
                         int baseMultiplier,
                         int itemMultiplier,
                         const vector<int>& contributingValues) {
    cout << "\033[H\033[J";

    cout << right << setw(70) << "SCORE    : " << player.getScore() << "\n";
    cout << right << setw(70) << "HANDS    : " << playRoundsLeft << "\n";
    cout << right << setw(70) << "DISCARDS : " << discardRoundsLeft << "\n\n";

    if (!handType.empty()) {
        cout << "THIS HAND: " << handType << "\n";

        ostringstream breakdown;
        for (size_t i = 0; i < contributingValues.size(); ++i) {
            breakdown << contributingValues[i];
            if (i + 1 != contributingValues.size()) breakdown << " + ";
        }

        cout << "SCORE CALC: (" << breakdown.str() << ") * "
             << baseMultiplier << " (hand type)";
        if (itemMultiplier > 1)
            cout << " * " << itemMultiplier << " (item)";
        cout << " = " << handScore << "\n\n";
    }

    cout << "╔════════════════════════════════════════════════════════════════════╗\n";
    cout << "║ Played Cards:                                                      ║\n";
    Hand ph;
    ph.setCards(played);
    ph.display(false);

    cout << "║ Hand:                                                              ║\n";

    vector<string> faces(8, "EMPTY");
    vector<string> suits(8, " ");
    vector<bool> isPlayed(8, false);

    for (int idx : playedIndices)
        if (idx >= 0 && idx < 8) isPlayed[idx] = true;

    size_t remainingIdx = 0;
    for (int i = 0; i < 8; ++i) {
        if (!isPlayed[i] && remainingIdx < remaining.size()) {
            faces[i] = remaining[remainingIdx].getFaceStr();
            suits[i] = remaining[remainingIdx].getSuitEmoji();
            ++remainingIdx;
        }
    }

    cout << "║   ";
    for (int i = 0; i < 8; ++i)
        cout << "  " << setw(2) << i << "    ";
    cout << " ║\n";

    cout << "║   ";
    for (int i = 0; i < 8; ++i)
        cout << "┌─────┐ ";
    cout << " ║\n";

    cout << "║   ";
    for (int i = 0; i < 8; ++i)
        if (faces[i] == "EMPTY")
            cout << "│EMPTY│ ";
        else
            cout << "│ " << setw(2) << faces[i] << "  │ ";
    cout << " ║\n";

    cout << "║   ";
    for (int i = 0; i < 8; ++i)
        if (faces[i] == "EMPTY")
            cout << "│     │ ";
        else
            cout << "│  " << suits[i] << "  │ ";
    cout << " ║\n";

    cout << "║   ";
    for (int i = 0; i < 8; ++i)
        cout << "└─────┘ ";
    cout << " ║\n";

    cout << "╚════════════════════════════════════════════════════════════════════╝\n";
}

void drawAwardScreen(const Player& player, int finalScore, int bestScoreBeforeUpdate) {
    const int boxWidth = 72;

    auto center = [](const string& text, int width) {
        int padding = (width - text.length()) / 2;
        return string(padding, ' ') + text + string(width - padding - text.length(), ' ');
    };

    cout << "\033[H\033[J";
    cout << "+" << string(boxWidth, '=') << "+\n";
    cout << "|" << center("** GAME OVER **", boxWidth) << "|\n";
    cout << "+" << string(boxWidth, '=') << "+\n";

    ostringstream scoreLine;
    scoreLine << "** " << finalScore << " **";
    cout << "|" << center("FINAL SCORE", boxWidth) << "|\n";
    cout << "|" << center(scoreLine.str(), boxWidth) << "|\n";

    cout << "|" << center("PERSONAL BEST", boxWidth) << "|\n";
    if (finalScore > bestScoreBeforeUpdate) {
        ostringstream newHigh;
        newHigh << finalScore << " (NEW HIGH SCORE!)";
        cout << "|" << center(newHigh.str(), boxWidth) << "|\n";
    } else {
        ostringstream bestLine;
        bestLine << bestScoreBeforeUpdate;
        cout << "|" << center(bestLine.str(), boxWidth) << "|\n";
    }

    cout << "+" << string(boxWidth, '-') << "+\n";

    cout << "|" << center("HAND STATS", boxWidth) << "|\n";
    const auto& stats = player.getStats();
    for (const auto& [type, count] : stats) {
        ostringstream line;
        line << type << " : " << count;
        cout << "|" << center(line.str(), boxWidth) << "|\n";
    }

    cout << "+" << string(boxWidth, '-') << "+\n";
    cout << "|" << center("LEADERBOARD", boxWidth) << "|\n";

    ostringstream buffer;
    auto* old_buf = cout.rdbuf(buffer.rdbuf());
    Leaderboard::displayTopPlayers(5, false);
    cout.rdbuf(old_buf);

    istringstream in(buffer.str());
    string line;
    while (getline(in, line)) {
        if (line.length() > boxWidth)
            line = line.substr(0, boxWidth - 3) + "...";
        cout << "|" << center(line, boxWidth) << "|\n";
    }

    cout << "+" << string(boxWidth, '=') << "+\n";
}
