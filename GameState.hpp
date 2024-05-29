#pragma once

#include "Player.hpp"
#include "Action.hpp"
#include <array>
#include <vector>
#include <ostream>

constexpr int STORE_MAX = 6;
constexpr int STORE_MIN = 13;
constexpr float MAX_SCORE = 1000;

class GameState {
private:
    std::array<int, 14> pits;
    Player turn;
public:
    GameState();
    GameState(const GameState &);
    GameState(const GameState &, Action a);

    ~GameState();

    int getPit(std::size_t index) const;
    Player getTurn() const;

    bool isTerminal() const;
    float score() const;

    std::vector<Action> getActions() const;
    // perform the current action on the GameState. Assume the GameState is not
    // terminal
    GameState &doAction(Action a);

    friend std::ostream &operator<<(std::ostream&, const GameState&);
};