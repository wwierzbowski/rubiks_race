#include "render.h"

#include <graphx.h>

#define TILE_SIZE (GFX_LCD_HEIGHT / BOARD_SIZE)
#define DEFAULT_TILE_SIZE_DECREASE 2
#define DEFAULT_TILE_OFFSET 1
#define GOAL_TILE_SCALAR 4

bool tile_is_significant(uint8_t x, uint8_t y) {
    if (x < 4 && x > 0 && y < 4 && y > 0) {
        return true;
    }
    return false;
}

Coord tile_coord_to_screen(uint8_t x, uint8_t y) {
    uint8_t offset = DEFAULT_TILE_OFFSET;
    if (tile_is_significant(x, y)) {
        offset *= GOAL_TILE_SCALAR;
    }
    Coord screen_coord = {
        x * TILE_SIZE + offset + (GFX_LCD_WIDTH - GFX_LCD_HEIGHT) / 2,
        y * TILE_SIZE + offset
    };

    return screen_coord;
}

uint8_t get_size_decrease(uint8_t x, uint8_t y) {
    uint8_t size_decrease = DEFAULT_TILE_SIZE_DECREASE;
    if (tile_is_significant(x, y)) {
        size_decrease *= GOAL_TILE_SCALAR;
    }

    return size_decrease;
}

void draw_tile(uint8_t x, uint8_t y, uint8_t color) {
    gfx_SetColor(color);
    uint8_t size_decrease = get_size_decrease(x, y);
    Coord screen_coord = tile_coord_to_screen(x, y);
    gfx_FillRectangle(screen_coord.x, screen_coord.y, TILE_SIZE - size_decrease, TILE_SIZE - size_decrease);
}

void draw_goal_tile(uint8_t x, uint8_t y, uint8_t color) {
    gfx_SetColor(color);
    Coord screen_coord = tile_coord_to_screen(x, y);
    screen_coord.x -= DEFAULT_TILE_OFFSET * GOAL_TILE_SCALAR;
    screen_coord.y -= DEFAULT_TILE_OFFSET * GOAL_TILE_SCALAR;
    gfx_FillRectangle(screen_coord.x, screen_coord.y, TILE_SIZE, TILE_SIZE);
}

void draw_moves_text(uint8_t moves) {
    gfx_SetColor(0);
    gfx_FillRectangle_NoClip(0, 0, 24, 8);
    gfx_SetTextXY(0, 0);
    gfx_PrintUInt(moves, 3);
}

void blit_draw_tile(uint8_t x, uint8_t y, uint8_t color) {
    draw_tile(x, y, color);
    Coord screen_coord = tile_coord_to_screen(x, y);
    uint8_t size_decrease = get_size_decrease(x, y);

    gfx_BlitRectangle(gfx_buffer, screen_coord.x, screen_coord.y, TILE_SIZE - size_decrease, TILE_SIZE - size_decrease);
}

void draw_board(uint8_t board[BOARD_SIZE][BOARD_SIZE], uint8_t goal[3][3]) {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (tile_is_significant(x, y)) {
                draw_goal_tile(x, y, goal[x - 1][y - 1]);
            }

            draw_tile(x, y, board[x][y]);
        }
    }
}
