#include "GameState.hpp"
#include <iostream>
#include <iomanip>
#include <random>

int main() {
    std::cout << "Hello world!\n";

    {
        GameState g;
        std::cout << g << " Score: " << g.score() << "\n";
        g.doAction(Action::C);
        std::cout << g << " Score: " << g.score() << "\n";
        g.doAction(Action::F);
        std::cout << g << " Score: " << g.score() << "\n";
    }

    std::cout << "\nRandom game:\n";

    std::random_device rd;
    std::mt19937 g(rd());
    typedef std::uniform_int_distribution<std::vector<Action>::size_type>
        uniform;
    uniform distrib(0);

    GameState game;
    std::cout << game << " Score: " << std::showpos << std::setw(5)
            << game.score() << std::endl << std::noshowpos;

    while (!game.isTerminal()) {
        std::vector<Action> actions = game.getActions();
        uniform::param_type params(0, actions.size() - 1);
        Action a = actions[distrib(g, params)];
        game.doAction(a);
        std::cout << game << " Score: " << std::showpos << std::setw(5)
                << game.score() << std::endl << std::noshowpos;
    }

    return 0;
}