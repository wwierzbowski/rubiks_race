#include "menu.h"

#include <stdbool.h>

#include <graphx.h>
#include <keypadc.h>

typedef struct {
    const char *label;
    uint8_t y;
    uint8_t color;
} MenuOption;

static const MenuOption MAIN_MENU_OPTIONS[] = {
    {"New Game", 80, 2},
    {"Set Seed", 120, 4},
    {"Profiles", 160, 6},
};

static const uint8_t MAIN_MENU_LEN = sizeof(MAIN_MENU_OPTIONS) / sizeof(MAIN_MENU_OPTIONS[0]);

void init_main_menu(void) {
    while (kb_Data[1] || kb_Data[6] || kb_Data[7]) {
        kb_Scan();
    }
}

static void draw_menu_title(void) {
    const char title[] = "Rubik's Race";

    gfx_SetTextXY(36, 10);
    gfx_SetTextScale(3, 3);

    for (int row = 0; row < 2; row++) {
        for (int index = 0; index < 6; index++) {
            gfx_SetTextFGColor(index + 1);
            gfx_PrintChar(title[index + (row * 6)]);
        }
    }
}

static void draw_menu_selection(uint8_t menu_selection_idx) {
    const MenuOption *option = &MAIN_MENU_OPTIONS[menu_selection_idx];

    gfx_SetColor(option->color);
    gfx_Rectangle_NoClip(96 - 4, option->y - 2, 8 * 2 * 8 - 4, 8 * 2 + 4);
}

static void draw_menu_options(void) {
    gfx_SetTextScale(2, 2);

    for (uint8_t index = 0; index < MAIN_MENU_LEN; index++) {
        gfx_SetTextFGColor(MAIN_MENU_OPTIONS[index].color);
        gfx_PrintStringXY(MAIN_MENU_OPTIONS[index].label, 96, MAIN_MENU_OPTIONS[index].y);
    }
}

static void draw_main_menu(uint8_t menu_selection_idx) {
    gfx_SetDrawBuffer();
    gfx_ZeroScreen();

    draw_menu_title();
    draw_menu_selection(menu_selection_idx);
    draw_menu_options();

    gfx_SwapDraw();
}

static void update_menu_selection(uint8_t *menu_selection_idx, kb_key_t arrows) {
    if (arrows == kb_Down && *menu_selection_idx < MAIN_MENU_LEN - 1) {
        (*menu_selection_idx)++;
    } else if (arrows == kb_Up && *menu_selection_idx > 0) {
        (*menu_selection_idx)--;
    }
}

int run_main_menu(void) {
    uint8_t menu_selection_idx = MENU_NEW_GAME;
    kb_key_t previous_arrows = 0;

    draw_main_menu(menu_selection_idx);

    while (true) {
        kb_Scan();

        if (kb_Data[6] == kb_Clear) {
            return -1;
        }

        if (kb_Data[6] == kb_Enter || kb_Data[1] == kb_2nd) {
            return menu_selection_idx;
        }

        kb_key_t arrows = kb_Data[7];
        if (arrows == previous_arrows) {
            continue;
        }

        previous_arrows = arrows;
        update_menu_selection(&menu_selection_idx, arrows);
        draw_main_menu(menu_selection_idx);
    }
}
