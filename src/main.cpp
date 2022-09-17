#include <madrona/context.hpp>

#include <fstream>

#include "game.hpp"

using namespace madrona;

namespace MadRogue {

static void launch()
{
    StateManager state_mgr;

    JobManager job_mgr(JobManager::makeEntry<Engine, Game>([](Engine &ctx) {
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

    MadRogue::launch();
}
