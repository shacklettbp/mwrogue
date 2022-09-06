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
    uint32_t hp;
};

struct Mana {
    float mp;
};

struct Quiver {
    uint32_t numArrows;
};

struct Action {
    float remainingTime;
};

struct Dragon : madrona::Archetype<Position, Health, Action, Mana> {};
struct Knight : madrona::Archetype<Position, Health, Action, Quiver> {};

}
