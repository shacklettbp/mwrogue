#pragma once

#include "components.hpp"

#include <madrona/custom_context.hpp>
#include <random>

namespace MadRogue {

class Engine;

struct Game {
    static void entry(Engine &ctx);

    Game(Engine &ctx);
    void gameLoop(Engine &ctx);
    void tick(Engine &ctx);

    madrona::Vector3 randomPosition();

    uint64_t tickCount;

    float deltaT;
    float moveSpeed;
    float manaRegenRate;
    float castTime;
    float shootTime;

    madrona::AABB worldBounds;

    madrona::Query<Position, Action> actionQuery;
    madrona::Query<Position, Health> healthQuery;
    madrona::Query<Action, Mana> casterQuery;
    madrona::Query<Action, Quiver> archerQuery;
    madrona::Query<madrona::Entity, Health> cleanupQuery;
};

class Engine : public::madrona::CustomContext<Engine> {
public:
    inline Engine(Game *game, madrona::WorkerInit &&init);

    inline Game & game() { return *game_; }

private:
    Game *game_;
};

}

#include "game.inl"
