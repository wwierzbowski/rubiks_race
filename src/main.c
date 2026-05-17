#include <graphx.h>
#include <keypadc.h>
#include <sys/util.h>
#include <sys/rtc.h>

#include "gfx/gfx.h"

//blue (1), yellow (2), green (3), orange (4), red (5), white (6)

#define BOARD_SIZE  5
#define TILE_SIZE (GFX_LCD_HEIGHT / BOARD_SIZE) //48
#define DEFAULT_TILE_SIZE_DECREASE 2
#define DEFAULT_TILE_OFFSET 1
#define GOAL_TILE_SCALAR 4

typedef struct Coord {
    int x;
    int y;
} Coord;

void coord_set_equal(Coord *coord, Coord *values) {
    coord->x = values->x;
    coord->y = values->y;
}

void swap_tiles(uint8_t *tile_1, uint8_t *tile_2) {
    uint8_t temp = *tile_1;
    *tile_1 = *tile_2;
    *tile_2 = temp;
}

bool tile_is_significant(uint8_t x, uint8_t y) {
    if (x < 4 && x > 0 &&  y < 4 && y > 0) {
        return true;
    }
    return false;
}

Coord tile_coord_to_screen(uint8_t x, uint8_t y) {
    uint8_t offset = DEFAULT_TILE_OFFSET;
    if (tile_is_significant(x, y)) {
        offset *= GOAL_TILE_SCALAR;
    }
    Coord screen_coord = {x * TILE_SIZE + offset + (GFX_LCD_WIDTH - GFX_LCD_HEIGHT) / 2, y * TILE_SIZE + offset};
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

bool is_in_bounds(uint8_t x, uint8_t y) {
    if (x >= 5 || x < 0 || y >= 5 || y < 0) return false;

    return true;
}

void blit_draw_tile(uint8_t x, uint8_t y, uint8_t color) {
    draw_tile(x ,y, color);
    Coord screen_coord = tile_coord_to_screen(x, y);
    uint8_t size_decrease = get_size_decrease(x, y);

    gfx_BlitRectangle(gfx_buffer, screen_coord.x, screen_coord.y, TILE_SIZE - size_decrease, TILE_SIZE - size_decrease);
}

bool is_win(uint8_t board[5][5], uint8_t goal[3][3]) {
    for (uint8_t x = 0; x < 3; x++) {
        for (uint8_t y = 0; y < 3; y++) {
            if (board[x + 1][y + 1] != goal[x][y]) return false;
        }
    }

    return true;
}

void shuffle_board(uint8_t board[5][5], Coord *empty) {
    // Shuffle
    for (uint8_t x = 0; x < BOARD_SIZE; x++) {
        for (uint8_t y = 0; y < BOARD_SIZE; y++) {
            uint8_t new_x = randInt(0, 4);
            uint8_t new_y = randInt(0, 4);

            swap_tiles(&board[x][y], &board[new_x][new_y]);
            if (!board[x][y]) {
                empty->x = x;
                empty->y = y;
            }
            else if (!board[new_x][new_y]) {
                empty->x = new_x;
                empty->y = new_y;
            }
        }
    }
}

void create_goal(uint8_t goal[3][3]) {
    // Create all color tiles
    uint8_t all_colors[24] = {1, 1, 1, 1, 6, 
                              2, 2, 2, 2, 6,
                              3, 3, 3, 3, 6,
                              4, 4, 4, 4, 6,
                              5, 5, 5, 5};

    // Shuffle around all color tiles
    for (uint8_t i = 0; i < 24; i++) {
        uint8_t new = randInt(0, 23);
        swap_tiles(&all_colors[i], &all_colors[new]);
    }

    // Take a selection of all color tiles
    for (uint8_t x = 0; x < 3; x++) {
        for (uint8_t y = 0; y < 3; y++) {
            goal[x][y] = all_colors[3*x + y];
        }
    }
}

void draw_board(uint8_t board[5][5], uint8_t goal[3][3]) {
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            // Draw goal tile backgrounds
            if (tile_is_significant(x, y)) {
                draw_goal_tile(x, y, goal[x - 1][y - 1]);
            }

            // Draw tile on top
            draw_tile(x, y, board[x][y]);
        }
    }

}

void load_new_game(uint8_t board[5][5], uint8_t goal[3][3], Coord *empty, unsigned int *moves) {
    shuffle_board(board, empty);
    create_goal(goal);
    draw_board(board, goal);

    *moves = 0;
}

int movement_update(kb_key_t *previous_arrows, Coord *direction, Coord *previous_direction, Coord *empty, uint8_t board[5][5]) {
    // -1 no move
    // 0 move, but don't update counter
    // 1 move, and update the move counter
    int move_change = 0;
    kb_Scan();
    kb_key_t arrows = kb_Data[7];

    direction->x = 0;
    direction->y = 0;

    if (arrows == *previous_arrows) 
        return 0;

    *previous_arrows = arrows;

    switch (arrows)
    {
        case kb_Down:
            direction->y = 1;
            break;
        case kb_Up:
            direction->y = -1;
            break;
        case kb_Left:
            direction->x = -1;
            break;
        case kb_Right:
            direction->x = 1;
            break;
        
        default:
            return move_change;
    }

    Coord new_empty = {empty->x + direction->x, empty->y + direction->y};
    if (!is_in_bounds(new_empty.x, new_empty.y)) return -1;

    if (direction->x != previous_direction->x || direction->y != previous_direction->y) 
        move_change = 1;

    coord_set_equal(previous_direction, direction);
    swap_tiles(&board[new_empty.x][new_empty.y], &board[empty->x][empty->y]);

    return move_change;
}

int main(void)
{
    srand(rtc_Time());
    uint8_t board[5][5] =  {{1, 1, 1, 1, 6}, 
                            {2, 2, 2, 2, 6},
                            {3, 3, 3, 3, 6},
                            {4, 4, 4, 4, 6},
                            {5, 5, 5, 5, 0}};

    uint8_t goal[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    Coord empty = {0, 0};
    unsigned int moves = 0;

    
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_ZeroScreen();

    gfx_SetTextXY(0, 0);
    gfx_SetTextFGColor(6);

    load_new_game(board, goal, &empty, &moves);

    kb_key_t previous_arrows = 0;
    Coord previous_direction = {0, 0};
    do
    {
        if (is_win(board, goal)) load_new_game(board, goal, &empty, &moves);

        Coord direction = {0, 0};

        int move_change = movement_update(&previous_arrows, &direction, &previous_direction, &empty, board);

        if (move_change == -1) continue;
        if (move_change) {
            moves++;
            draw_moves_text(moves);
        }

        if (is_in_bounds(empty.x, empty.y) && is_in_bounds(empty.x + direction.x, empty.y + direction.y))
        {
            gfx_SetDrawBuffer();

            blit_draw_tile(empty.x, empty.y, board[empty.x][empty.y]);
            blit_draw_tile(empty.x + direction.x, empty.y + direction.y, board[empty.x + direction.x][empty.y + direction.y]);

            // Update empty coord tracker
            Coord new_empty = {empty.x + direction.x, empty.y + direction.y};
            coord_set_equal(&empty, &new_empty);

            gfx_SetDrawScreen();
            
        }
    } while (kb_Data[6] != kb_Clear);
    

    gfx_End();

    return 0;
}
