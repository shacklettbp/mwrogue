#include <madrona/context.hpp>

#include <fstream>

#include "game.hpp"

using namespace madrona;

namespace MadRogue {

static void launch(int num_worlds)
{
    StateManager state_mgr(num_worlds);

    JobManager job_mgr(JobManager::makeEntry<Engine, Game>([](Engine &ctx) {
            Game::entry(ctx);
        }), 0, 0, &state_mgr);

    job_mgr.waitForAllFinished();

    printf("Done\n");
}

}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        FATAL("Usage: %s NUM_WORLDS", argv[0]);
    }

    int num_worlds = std::stoi(argv[1]);

    if (num_worlds < 1) {
        FATAL("%s: num worlds must be greater than 0");
    }

    MadRogue::launch(num_worlds);
}
