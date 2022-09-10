#pragma once

#include <madrona/geometry.hpp>
#include <madrona/ecs.hpp>

namespace MWRogue {

struct Position : madrona::Vector3 {
    Position(Vector3 o)
        : Vector3(o)
    {}
};

struct Health {
    int hp;
};

struct Mana {
    float mp;
};

struct Quiver {
    int numArrows;
};

struct Action {
    float remainingTime;
};

struct CleanupEntity : madrona::Entity {
    CleanupEntity(madrona::Entity e)
        : Entity(e)
    {}
};

struct Dragon : madrona::Archetype<Position, Health, Action, Mana> {};
struct Knight : madrona::Archetype<Position, Health, Action, Quiver> {};
struct CleanupTracker : madrona::Archetype<CleanupEntity> {};

}
