#include "game.hpp"

namespace MWRogue {

static void update(Game &game)
{
    printf("Hi\n");
}

void World::entry(Game &game)
{
    JobID update_id = game.submit([](Game &game) {
        update(game);

        game.submit([](Game &game) {
            update(game);
        }, false, game.jobID());
    }, false);
}

}
