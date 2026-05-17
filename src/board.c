#include "board.h"

#include <sys/util.h>

void coord_set_equal(Coord *coord, const Coord *values) {
    coord->x = values->x;
    coord->y = values->y;
}

void swap_tiles(uint8_t *tile_1, uint8_t *tile_2) {
    uint8_t temp = *tile_1;
    *tile_1 = *tile_2;
    *tile_2 = temp;
}

bool is_in_bounds(int x, int y) {
    if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0) {
        return false;
    }

    return true;
}

bool is_win(uint8_t board[BOARD_SIZE][BOARD_SIZE], uint8_t goal[3][3]) {
    for (uint8_t x = 0; x < 3; x++) {
        for (uint8_t y = 0; y < 3; y++) {
            if (board[x + 1][y + 1] != goal[x][y]) {
                return false;
            }
        }
    }

    return true;
}

void shuffle_board(uint8_t board[BOARD_SIZE][BOARD_SIZE], Coord *empty) {
    for (uint8_t x = 0; x < BOARD_SIZE; x++) {
        for (uint8_t y = 0; y < BOARD_SIZE; y++) {
            uint8_t new_x = randInt(0, BOARD_SIZE - 1);
            uint8_t new_y = randInt(0, BOARD_SIZE - 1);

            swap_tiles(&board[x][y], &board[new_x][new_y]);
            if (!board[x][y]) {
                empty->x = x;
                empty->y = y;
            } else if (!board[new_x][new_y]) {
                empty->x = new_x;
                empty->y = new_y;
            }
        }
    }
}

void create_goal(uint8_t goal[3][3]) {
    uint8_t all_colors[24] = {
        1, 1, 1, 1, 6,
        2, 2, 2, 2, 6,
        3, 3, 3, 3, 6,
        4, 4, 4, 4, 6,
        5, 5, 5, 5
    };

    for (uint8_t i = 0; i < 24; i++) {
        uint8_t new_index = randInt(0, 23);
        swap_tiles(&all_colors[i], &all_colors[new_index]);
    }

    for (uint8_t x = 0; x < 3; x++) {
        for (uint8_t y = 0; y < 3; y++) {
            goal[x][y] = all_colors[3 * x + y];
        }
    }
}

void init_board(uint8_t board[BOARD_SIZE][BOARD_SIZE]) {
    uint8_t initial[BOARD_SIZE][BOARD_SIZE] = {
        {1, 1, 1, 1, 6},
        {2, 2, 2, 2, 6},
        {3, 3, 3, 3, 6},
        {4, 4, 4, 4, 6},
        {5, 5, 5, 5, 0}
    };

    for (uint8_t x = 0; x < BOARD_SIZE; x++) {
        for (uint8_t y = 0; y < BOARD_SIZE; y++) {
            board[x][y] = initial[x][y];
        }
    }
}
