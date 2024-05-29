#include "GameState.hpp"
#include <string>
#include <iomanip>

GameState::GameState() : turn(Player::MAX) {
    pits = {4, 4, 4, 4, 4, 4, 0,
            4, 4, 4, 4, 4, 4, 0};
}

GameState::GameState(const GameState &other) : turn(other.turn), 
        pits(other.pits) {}

GameState::GameState(const GameState &state, Action a) : turn(state.turn),
        pits(state.pits) {
    doAction(a);
}

GameState::~GameState() {}

int GameState::getPit(std::size_t index) const {
    return pits[index];
}

Player GameState::getTurn() const {
    return turn;
}

bool GameState::isTerminal() const {
    bool allEmpty = true;
    for (int i = 0; i < STORE_MAX; ++i) {
        if (pits[i] > 0) {
            allEmpty = false;
            break;
        }
    }

    if (allEmpty) return true;

    for (int i = STORE_MAX + 1; i < STORE_MIN; ++i) {
        if (pits[i] > 0)
            return false;
    }

    return true;
}

float GameState::score() const {
    int val = 0;

    for (int i = 0; i <= STORE_MAX; ++i) {
        val += pits[i];
    }

    // 24 is the total amount of seeds (4*12) / 2.
    val -= 24;

    if (val == 0) return 0.0f;

    if (isTerminal()) {
        return val > 0 ? MAX_SCORE : -MAX_SCORE;
    }
    return static_cast<float>(val);
}

std::vector<Action> GameState::getActions() const {
    std::vector<Action> actions;
    int start;
    int actionOffset;
    if (turn == Player::MAX) {
        start = 0;
        actionOffset = Action::A;
    } else {
        start = STORE_MAX + 1;
        actionOffset = Action::a;
    }

    for (int i = 0; i < 6; ++i) {
        int index = i + start;
        if (pits[index] > 0)
            actions.push_back(static_cast<Action>(i + actionOffset));
    }

    return actions;
}

GameState &GameState::doAction(Action a) {
    if ((turn == Player::MAX && a >= Action::a)
            || (turn == Player::MIN && a < Action::a)) {
        throw std::string("Invalid action on GameState! (Wrong player action)");
    }
    // i is the starting index from the given action
    int i;
    if (a < Action::a)
        i = a;
    else
        i = a - Action::a + STORE_MAX + 1;
    
    int hold = pits[i];
    if (hold == 0)
        throw std::string("Invalid action on GameState! (Action on empty pit)");
    pits[i] = 0;

    // distribute seeds 1-by-1
    while (hold > 0) {
        ++i;
        // if the pit is the opposite player's store, skip it
        if ((turn == Player::MAX && i == STORE_MIN)
                || (turn == Player::MIN && i == STORE_MAX)) {
            ++i;
        }

        // past the last pit, loop back to 0
        if (i >= pits.size()) {
            i = 0;
        }

        // place seed into pit
        --hold;
        ++pits[i];
    }

    // if we ended in a store, we're done without swapping the turn
    if (i == STORE_MAX || i == STORE_MIN) return *this;

    // check if we landed on our side of the board
    if ((turn == Player::MAX && i < STORE_MAX) || (turn == Player::MIN && i > STORE_MAX)) {
        // find the index of the pit on the opposite side of the board
        int opposite = 12 - i;
        // check if steal is possible
        if (pits[i] == 1 && pits[opposite] > 0) {
            // index of our store location
            int store = turn == Player::MAX ? STORE_MAX : STORE_MIN;
            
            // add to this player's store
            pits[store] += pits[i];
            pits[store] += pits[opposite];

            // remove from pits
            pits[i] = 0;
            pits[opposite] = 0;
        }
    }

    // swap turn order
    turn = static_cast<Player>(1 - turn);

    return *this;
}

std::ostream &operator<<(std::ostream &strm, const GameState &state) {
    strm << "Pits:";
    for (int pit : state.pits) {
        strm << " " << std::setw(2) << pit;
    }
    strm << " Turn: " << (state.turn == Player::MAX ? "P1" : "P2");
    return strm;
}