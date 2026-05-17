#include <graphx.h>
#include <keypadc.h>
#include <sys/rtc.h>

#include "board.h"
#include "gfx/gfx.h"
#include "input.h"
#include "render.h"

void load_new_game(uint8_t board[BOARD_SIZE][BOARD_SIZE], uint8_t goal[3][3], Coord *empty, unsigned int *moves) {
    shuffle_board(board, empty);
    create_goal(goal);
    draw_board(board, goal);

    *moves = 0;
}

int main(void)
{
    srand(rtc_Time());
    uint8_t board[BOARD_SIZE][BOARD_SIZE];

    uint8_t goal[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    Coord empty = {0, 0};
    unsigned int moves = 0;

    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_ZeroScreen();

    gfx_SetTextXY(0, 0);
    gfx_SetTextFGColor(6);

    init_board(board);
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
