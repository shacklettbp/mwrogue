#include "game.hpp"

#include "components.hpp"

using namespace madrona;

namespace MWRogue {

// FIXME
static std::mt19937 & randGen()
{
    thread_local std::mt19937 rand_gen { std::random_device {}() };

    return rand_gen;
}

Vector3 Game::randomPosition()
{
    std::uniform_real_distribution<float> x_dist(worldBounds.pMin.x, worldBounds.pMax.x);
    std::uniform_real_distribution<float> y_dist(worldBounds.pMin.y, worldBounds.pMax.y);
    std::uniform_real_distribution<float> z_dist(worldBounds.pMin.z, worldBounds.pMax.z);
    
    return Vector3 {
        x_dist(randGen()),
        y_dist(randGen()),
        z_dist(randGen()),
    };
}

void Game::init(Engine &ctx)
{
    tickCount = 0;
    deltaT = 1.f / 60.f;
    moveSpeed = 0.1f; // Move speed in m / s
    manaRegenRate = 1.f;
    castTime = 2.f;
    shootTime = 0.5f;

    worldBounds = {
        .pMin = Vector3 { -10, -10, 0, },
        .pMax = Vector3 { 10, 10, 10, },
    };

    ctx.state().registerComponent<Position>();
    ctx.state().registerComponent<Health>();
    ctx.state().registerComponent<Action>();
    ctx.state().registerComponent<Mana>();
    ctx.state().registerComponent<Quiver>();

    ctx.state().registerArchetype<Dragon>();
    ctx.state().registerArchetype<Knight>();

    actionQuery = ctx.query<Position, Action>();
    healthQuery = ctx.query<Position, Health>();
    casterQuery = ctx.query<Action, Mana>();
    archerQuery = ctx.query<Action, Quiver>();

    const int init_num_dragons = 10;
    const int init_num_knights = 20;
    const int dragon_hp = 1000;
    const int knight_hp = 100;

    std::uniform_real_distribution<float> mp_dist(0.f, 50.f);
    std::uniform_int_distribution<uint32_t> arrows_dist(0, 100);

    for (int i = 0; i < init_num_dragons; i++) {
        Position rand_pos = randomPosition();
        Health health { dragon_hp };
        Action act { 0 };
        Mana mp { mp_dist(randGen()) };

        ctx.state().makeEntity<Dragon>(rand_pos, health, act, mp);
    }

    for (int i = 0; i < init_num_knights; i++) {
        Position rand_pos = randomPosition();
        Health health { knight_hp };
        Action act { 0 };
        Quiver q { arrows_dist(randGen()) };

        ctx.state().makeEntity<Knight>(rand_pos, health, act, q);
    }
}

void Game::tick(Engine &ctx)
{
    JobID init_action_job = ctx.forAll(actionQuery, [this](Engine &ctx,
                                                           Position &pos,
                                                           Action &action) {
        if (action.remainingTime > 0) {
            action.remainingTime -= deltaT;
            return;
        }
        
        std::uniform_real_distribution<float> action_prob(0.f, 1.f);

        float move_cutoff = 0.5f;

        if (action_prob(randGen()) <= move_cutoff) {
            ctx.submit([this, &pos, &action](Engine &) {

                // Move
                std::uniform_real_distribution<float> pos_dist(-1.f, 1.f);

                Vector3 new_pos = pos + Vector3 {
                    pos_dist(randGen()),
                    pos_dist(randGen()),
                    pos_dist(randGen()),
                };

                new_pos.x = std::clamp(new_pos.x, worldBounds.pMin.x, worldBounds.pMax.x);
                new_pos.y = std::clamp(new_pos.y, worldBounds.pMin.y, worldBounds.pMax.y);
                new_pos.z = std::clamp(new_pos.x, worldBounds.pMin.z, worldBounds.pMax.z);

                Vector3 pos_delta = new_pos - pos;
                pos = new_pos;

                action.remainingTime = pos_delta.length() / moveSpeed;
            });
        } 
    });

    JobID cast_job = ctx.forAll(casterQuery, [this](Engine &ctx,
                                                    Action &action,
                                                    Mana &mana) {
        mana.mp += manaRegenRate * deltaT;

        if (action.remainingTime > 0) {
            return;
        }
        // move job runs first so if remainingTime == 0, always act (otherwise would do nothing)

        const float cast_cost = 20.f;

        if (mana.mp < cast_cost) {
            return;
        }

        mana.mp -= cast_cost;

        auto target_pos = randomPosition();

        ctx.forAll(healthQuery, [target_pos](Engine &,
                                             const Position &pos,
                                             Health &health) {
            const float blast_radius = 2.f;
            const float damage = 20.f;

            if (target_pos.distance(pos) <= blast_radius) {
                health.hp -= damage;
            }
        });

        action.remainingTime = castTime;
    }, true, init_action_job);

    JobID archer_job = ctx.forAll(archerQuery, [](Engine &ctx,
                                                  Action &action,
                                                  Quiver &quiver) {
        if (action.remainingTime > 0) {
            return;
        }

        auto dragons = ctx.archetype<Dragon>();
        uint32_t num_dragons = dragons.size();

        std::uniform_int_distribution<uint32_t> dragon_sel(0, num_dragons - 1);

        uint32_t dragon_idx = dragon_sel(randGen());
        Health &dragon_health = dragons.get<Health>(Loc { dragon_idx });

        const float damage = 15.f;
        dragon_health.hp -= damage;

        quiver.numArrows -= 1;
    }, true, init_action_job);

    ctx.submit([this](Engine &ctx) {
        ctx.state().iterateEntities(healthQuery, [](const Position &,
                                                    Health &health) {
            if (health.hp <= 0) {
            }
        });
    }, true, cast_job, archer_job);
}

void Game::gameLoop(Engine &ctx)
{
    ctx.submit([this](Engine &ctx) {
        tick(ctx);

        tickCount += 1;

        auto dragons = ctx.archetype<Dragon>();
        auto knights = ctx.archetype<Knight>();

        if (dragons.size() == 0) {
            printf("Dragons win!\n");
            return;
        }

        if (knights.size() == 0) {
            printf("Knights win!\n");
            return;
        }

        gameLoop(ctx);
    }, false, ctx.currentJobID());
}

void Game::entry(Engine &ctx)
{
    Game &game = ctx.game();
    // Initialization
    game.init(ctx);

    // Start game loop
    game.gameLoop(ctx);
}

}
