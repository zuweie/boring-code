#include "action.h"
#include "grid_world.h"
#include "agent.h"

int agent_init(agent_t* agent, grid_world_t* world)
{
    agent->world = world;
    agent->curr_stat = e_idle;
    agent->policies = malloc(world->rows * world->cols * sizeof(action_t));
    return 0;
}
int agent_reset(agent_t* agent)
{
    free(agent->policies);
    return 0;
}

int do_action(agent_t* agent, action_t action)
{
    return 0;
}