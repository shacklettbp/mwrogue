#include "game.hpp"

using namespace madrona;

namespace MWRogue {

static inline void update(Game &)
{
    printf("Hi\n");
}

static void updateLoop(Game &game)
{
    update(game);

    game.submit([](Game &game) {
        updateLoop(game);
    }, false, game.currentJobID());
}

void World::entry(Game &game)
{
    // Initialization

    // Start game loop
    game.submit([](Game &game) {
        updateLoop(game);
    }, false);
}

}
