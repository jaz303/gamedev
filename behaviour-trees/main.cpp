#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define JBT_DEBUG 1
#include "jbt.h"

const int MAP_SIZE          = 32;
const char EMPTY            = '.';
const char CLOSED_DOOR      = 'D';
const char OPEN_DOOR        = 'd';
const char SMASHED_DOOR     = 's';
const char LOCKED_DOOR      = 'L';
const char GOAL             = 'X';
const int BRAIN_MEMORY      = 512;

struct level_t {
    char map[MAP_SIZE + 1];
    int player_pos;
    jbt_brain_t player_brain;
    char player_brain_memory[BRAIN_MEMORY];
};

const int NUM_LEVELS = 10;

int g_iteration = 0;
level_t g_levels[NUM_LEVELS];

int is_goal(char tile) {
    return tile == GOAL;
}

int is_impassable_door(char tile) {
    return tile == CLOSED_DOOR || tile == LOCKED_DOOR;
}

char player_get_tile(level_t *level) {
    return level->map[level->player_pos];
}

char player_look_right(level_t *level) {
    return EMPTY;
}

int player_try_move_right(level_t *level) {
    int did_move = 0;
    if (level->player_pos < MAP_SIZE - 1) {
        char next_tile = level->map[level->player_pos+1];
        if (next_tile == EMPTY || next_tile == OPEN_DOOR || next_tile == GOAL) {
            level->player_pos++;
            did_move = 1;
        }
    }
    return did_move;
}

void clear_screen() {
    system("clear");
}

void init() {
    for (int i = 0; i < NUM_LEVELS; ++i) {
        level_t *level = &g_levels[i];
        for (int j = 0; j < MAP_SIZE; ++j) {
            level->map[j] = EMPTY;
        }
        int door_position = 3 + (rand() % (MAP_SIZE - 6));
        int choice = rand() % 3;
        char door_type;
        if (choice == 0) {
            door_type = CLOSED_DOOR;
        } else if (choice == 1) {
            door_type = LOCKED_DOOR;
        } else {
            door_type = OPEN_DOOR;
        }
        level->map[door_position] = door_type;
        level->map[MAP_SIZE-1] = GOAL;
        level->map[MAP_SIZE] = 0;
        level->player_pos = 0;
        jbt_init_brain(
            &level->player_brain,
            level->player_brain_memory,
            BRAIN_MEMORY
        );
    }
}

void update() {
    for (int i = 0; i < NUM_LEVELS; ++i) {
        jbt_tick_brain(&g_levels[i].player_brain);
        player_try_move_right(&g_levels[i]);
    }
}

void render() {
    clear_screen();
    puts("+---------------------+");
    puts("| Behaviour Tree Demo |");
    puts("+---------------------+\n");
    printf("Iteration: %d\n\n", g_iteration);
    for (int i = 0; i < NUM_LEVELS; ++i) {
        level_t *level = &g_levels[i];
        printf("%02d ", i);
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (j == level->player_pos) {
                putc('@', stdout);
            } else {
                putc(level->map[j], stdout);
            }
        }
        putc('\n', stdout);
    }
    putc('\n', stdout);
}

//
// Behaviours


JBT_BEGIN_LEAF(walk)
    JBT_STATE() 
    JBT_INIT()
    JBT_TICK()
JBT_END_LEAF()

JBT_LEAF(walk, level_t*) {
    // if (is_goal(player_tile(entity))) {
    //     return JBT_SUCCESS;
    // }
    // if (is_impassable_door(player_look_right(entity))) {
    //     return JBT_SUCCESS;
    // } else {
    //     player_try_move_right(entity);
    // }
    return JBT_SUCCESS;
}

JBT_LEAF(open_door, level_t*) {
    return JBT_SUCCESS;
}

struct smash_door_state_t {
    jbt_state_t state;
    int count;
};

JBT_STATEFUL_LEAF(smash_door, level_t*, smash_door_state_t*) {
    // it takes a while to smash a door down
    if (state->count >= 2) {
        return JBT_SUCCESS;
    } else {
        state->count++;
        return JBT_RUNNING;
    }
}

JBT_LEAF(close_door, level_t*) {
    return JBT_SUCCESS;
}

JBT_LEAF(walk_to_goal, level_t*) {
    return JBT_SUCCESS;
}

//
// Main

int main(int argc, char *argv[]) {

    time_t tmp_time;
    srand(time(&tmp_time));

    init();

    while (1) {
        g_iteration++;
        update();
        render();   
        usleep(200000);
    }
    
    return 0;

}