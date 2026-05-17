#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include <stdint.h>

#include "game_types.h"

bool tile_is_significant(uint8_t x, uint8_t y);
Coord tile_coord_to_screen(uint8_t x, uint8_t y);
uint8_t get_size_decrease(uint8_t x, uint8_t y);
void draw_tile(uint8_t x, uint8_t y, uint8_t color);
void draw_goal_tile(uint8_t x, uint8_t y, uint8_t color);
void draw_moves_text(uint8_t moves);
void blit_draw_tile(uint8_t x, uint8_t y, uint8_t color);
void draw_board(uint8_t board[BOARD_SIZE][BOARD_SIZE], uint8_t goal[3][3]);

#endif
