#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int MAP_SIZE = 32;
const char EMPTY            = '.';
const char CLOSED_DOOR      = 'D';
const char OPEN_DOOR        = 'd';
const char SMASHED_DOOR     = 's';
const char LOCKED_DOOR      = 'L';
const char GOAL             = 'X';

struct level_t {
    char map[MAP_SIZE + 1];
    int player_pos;
};

const int NUM_LEVELS = 10;

int g_iteration = 0;
level_t g_levels[NUM_LEVELS];

int try_move_player_right(level_t *level) {
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
    }
}

void update() {
    for (int i = 0; i < NUM_LEVELS; ++i) {
        try_move_player_right(&g_levels[i]);
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