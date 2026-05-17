#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

#include <keypadc.h>

#include "game_types.h"

int movement_update(kb_key_t *previous_arrows,
                    Coord *direction,
                    Coord *previous_direction,
                    Coord *empty,
                    uint8_t board[BOARD_SIZE][BOARD_SIZE]);

#endif
