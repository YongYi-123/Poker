#include "Display.h"
#include "Player.h"
#include "Card.h"
#include "Hand.h"
#include "Leaderboard.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
void drawHandOnlyInterface(const Player& player,
                           const std::vector<Card>& handCards,
                           int playRoundsLeft,
                           int discardRoundsLeft) {
    std::cout << "\033[H\033[J";  // Clear and move cursor to top

    std::cout << std::right << std::setw(70) << "SCORE    : " << player.getScore() << "\n";
    std::cout << std::right << std::setw(70) << "HANDS    : " << playRoundsLeft << "\n";
    std::cout << std::right << std::setw(70) << "DISCARDS : " << discardRoundsLeft << "\n\n";

    std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";

    // Index line (aligned)
    std::cout << "║   ";
    for (size_t i = 0; i < handCards.size(); ++i)
        std::cout << "  " << std::setw(2) << i << "    ";
    std::cout << std::string((8 - handCards.size()) * 8, ' ') << " ║\n";

    std::cout << "║   ";
    for (size_t i = 0; i < handCards.size(); ++i)
        std::cout << "┌─────┐ ";
    std::cout << std::string((8 - handCards.size()) * 8, ' ') << " ║\n";

    std::cout << "║   ";
    for (const Card& card : handCards)
        std::cout << "│ " << std::setw(2) << card.getFaceStr() << "  │ ";
    std::cout << std::string((8 - handCards.size()) * 8, ' ') << " ║\n";

    std::cout << "║   ";
    for (const Card& card : handCards)
        std::cout << "│  " << card.getSuitEmoji() << "  │ ";
    std::cout << std::string((8 - handCards.size()) * 8, ' ') << " ║\n";

    std::cout << "║   ";
    for (size_t i = 0; i < handCards.size(); ++i)
        std::cout << "└─────┘ ";
    std::cout << std::string((8 - handCards.size()) * 8, ' ') << " ║\n";

    std::cout << "╚════════════════════════════════════════════════════════════════════╝\n";
}


void drawResultInterface(
    const Player& player,
    const std::vector<Card>& played,
    const std::vector<Card>& remaining,
    const std::vector<int>& playedIndices,
    int playRoundsLeft,
    int discardRoundsLeft,
    const std::string& handType,
    int handScore,
    int baseMultiplier,
    int itemMultiplier,
    const std::vector<int>& contributingValues
) {
    // ANSI escape codes: move to top-left and clear screen from cursor down
    std::cout << "\033[H\033[J";

    // Top-right stats
    std::cout << std::right << std::setw(70) << "SCORE    : " << player.getScore() << "\n";
    std::cout << std::right << std::setw(70) << "HANDS    : " << playRoundsLeft << "\n";
    std::cout << std::right << std::setw(70) << "DISCARDS : " << discardRoundsLeft << "\n\n";

    // Show hand type result if any
    if (!handType.empty()) {
        std::cout << "THIS HAND: " << handType << "\n";

        // Detailed score breakdown (only contributing cards)
        std::ostringstream breakdown;
        for (size_t i = 0; i < contributingValues.size(); ++i) {
            breakdown << contributingValues[i];
            if (i + 1 != contributingValues.size()) breakdown << " + ";
        }

        std::cout << "SCORE CALC: (" << breakdown.str() << ") * "
                  << baseMultiplier << " (hand type)";
        if (itemMultiplier > 1)
            std::cout << " * " << itemMultiplier << " (item)";
        std::cout << " = " << handScore << "\n\n";
    }

    // Outer box top
    std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";

    // Played cards section
    std::cout << "║ Played Cards:                                                      ║\n";
    Hand ph;
    ph.setCards(played);
    ph.display(false);

    // Remaining hand section
    std::cout << "║ Hand:                                                              ║\n";

    std::vector<std::string> faces(8, "EMPTY");
    std::vector<std::string> suits(8, " ");
    std::vector<bool> isPlayed(8, false);

    for (int idx : playedIndices) {
        if (idx >= 0 && idx < 8) isPlayed[idx] = true;
    }

    size_t remainingIdx = 0;
    for (int i = 0; i < 8; ++i) {
        if (!isPlayed[i] && remainingIdx < remaining.size()) {
            faces[i] = remaining[remainingIdx].getFaceStr();
            suits[i] = remaining[remainingIdx].getSuitEmoji();
            ++remainingIdx;
        }
    }

    // Index row
    std::cout << "║   ";
    for (int i = 0; i < 8; ++i)
        std::cout << "  " << std::setw(2) << i << "    ";
    std::cout << " ║\n";

    // Top borders
    std::cout << "║   ";
    for (int i = 0; i < 8; ++i)
        std::cout << "┌─────┐ ";
    std::cout << " ║\n";

    // Face values
    std::cout << "║   ";
    for (int i = 0; i < 8; ++i) {
        if (faces[i] == "EMPTY")
            std::cout << "│" << std::setw(5) << "EMPTY" << "│ ";
        else
            std::cout << "│ " << std::setw(2) << faces[i] << "  │ ";
    }
    std::cout << " ║\n";

    // Suits
    std::cout << "║   ";
    for (int i = 0; i < 8; ++i) {
        if (faces[i] == "EMPTY")
            std::cout << "│     │ ";
        else
            std::cout << "│  " << suits[i] << "  │ ";
    }
    std::cout << " ║\n";

    // Bottom borders
    std::cout << "║   ";
    for (int i = 0; i < 8; ++i)
        std::cout << "└─────┘ ";
    std::cout << " ║\n";

    // Bottom of outer box
    std::cout << "╚════════════════════════════════════════════════════════════════════╝\n";
}

void drawAwardScreen(const Player& player, int finalScore, int bestScore) {
    const int boxWidth = 72;

    auto center = [](const std::string& text, int width) {
        int padding = (width - text.length()) / 2;
        return std::string(padding, ' ') + text + std::string(width - padding - text.length(), ' ');
    };

    std::cout << "\033[H\033[J";

    std::cout << "+" << std::string(boxWidth, '=') << "+\n";
    std::cout << "|" << center("** GAME OVER **", boxWidth) << "|\n";
    std::cout << "+" << std::string(boxWidth, '=') << "+\n";

    // Final score section
    std::ostringstream scoreLine;
    scoreLine << "** " << finalScore << " **";
    std::cout << "|" << center("FINAL SCORE", boxWidth) << "|\n";
    std::cout << "|" << center(scoreLine.str(), boxWidth) << "|\n";

    std::ostringstream bestLine;
    bestLine << bestScore;
    std::cout << "|" << center("PERSONAL BEST", boxWidth) << "|\n";
    std::cout << "|" << center(bestLine.str(), boxWidth) << "|\n";

    std::cout << "+" << std::string(boxWidth, '-') << "+\n";

    // Hand Stats section
    std::cout << "|" << center("HAND STATS", boxWidth) << "|\n";
    const auto& stats = player.getStats();
    for (const auto& [type, count] : stats) {
        std::ostringstream line;
        line << type << " : " << count;
        std::cout << "|" << center(line.str(), boxWidth) << "|\n";
    }

    std::cout << "+" << std::string(boxWidth, '-') << "+\n";

    // Leaderboard section
    std::cout << "|" << center("LEADERBOARD", boxWidth) << "|\n";

    std::ostringstream buffer;
    auto* old_buf = std::cout.rdbuf(buffer.rdbuf());
    Leaderboard::displayTopPlayers(5, false);  // capture output
    std::cout.rdbuf(old_buf);

    std::istringstream in(buffer.str());
    std::string line;
    while (std::getline(in, line)) {
        if (line.length() > boxWidth)
            line = line.substr(0, boxWidth - 3) + "...";
        std::cout << "|" << center(line, boxWidth) << "|\n";
    }

    std::cout << "+" << std::string(boxWidth, '=') << "+\n";
}