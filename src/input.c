#include "input.h"

#include "board.h"

int movement_update(kb_key_t *previous_arrows,
                    Coord *direction,
                    Coord *previous_direction,
                    Coord *empty,
                    uint8_t board[BOARD_SIZE][BOARD_SIZE]) {
    int move_change = 0;
    kb_Scan();
    kb_key_t arrows = kb_Data[7];

    direction->x = 0;
    direction->y = 0;

    if (arrows == *previous_arrows) {
        return 0;
    }

    *previous_arrows = arrows;

    switch (arrows) {
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
    if (!is_in_bounds(new_empty.x, new_empty.y)) {
        return -1;
    }

    if (direction->x != previous_direction->x || direction->y != previous_direction->y) {
        move_change = 1;
    }

    coord_set_equal(previous_direction, direction);
    swap_tiles(&board[new_empty.x][new_empty.y], &board[empty->x][empty->y]);

    return move_change;
}
