#pragma once

#include <madrona/custom_context.hpp>

namespace MWRogue {

class Game;

struct World {
    static void entry(Game &game);
};

class Game : public::madrona::CustomContext<Game> {
public:
    inline Game(World *world, madrona::WorkerInit &&init);

    inline World & world() { return *world_; }

private:
    World *world_;
};

}

#include "game.inl"
