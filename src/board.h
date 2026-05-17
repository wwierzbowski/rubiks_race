#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "game_types.h"

void coord_set_equal(Coord *coord, const Coord *values);
void swap_tiles(uint8_t *tile_1, uint8_t *tile_2);
bool is_in_bounds(int x, int y);
bool is_win(uint8_t board[BOARD_SIZE][BOARD_SIZE], uint8_t goal[3][3]);
void shuffle_board(uint8_t board[BOARD_SIZE][BOARD_SIZE], Coord *empty);
void create_goal(uint8_t goal[3][3]);
void init_board(uint8_t board[BOARD_SIZE][BOARD_SIZE]);

#endif
