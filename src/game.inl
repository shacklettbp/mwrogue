#pragma once

namespace MadRogue {

Engine::Engine(Game *game, madrona::WorkerInit &&init)
    : madrona::CustomContext<Engine>(std::move(init)),
      game_(game)
{}

}
