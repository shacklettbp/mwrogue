#include <madrona/context.hpp>

#include <fstream>

#include "game.hpp"

using namespace madrona;

namespace MWRogue {

static void launch()
{
    StateManager state_mgr;

    Optional<Game> game = Optional<Game>::none();

    JobManager job_mgr(JobManager::makeEntry<Engine>(
        std::move(*game), [](Engine &ctx) {
            Game::entry(ctx);
        }), 0, 0, &state_mgr);

    job_mgr.waitForAllFinished();

    printf("Done\n");
}

}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    MWRogue::launch();
}
