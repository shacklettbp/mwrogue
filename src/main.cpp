#include <madrona/context.hpp>

#include <fstream>

#include "game.hpp"

using namespace madrona;
using namespace std;

namespace MWRogue {

static void launch()
{
    StateManager state_mgr;

    World world;

    JobManager job_mgr(JobManager::makeEntry<Game>(
        std::move(world), [](Game &game) {
            World::entry(game);
        }), 0, 0, &state_mgr);

    job_mgr.waitForAllFinished();
}

}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    MWRogue::launch();
}
