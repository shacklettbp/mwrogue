#pragma once

namespace MWRogue {

Game::Game(World *world, madrona::WorkerInit &&init)
    : madrona::CustomContext<Game>(std::move(init)),
      world_((World *)world)
{}

}
