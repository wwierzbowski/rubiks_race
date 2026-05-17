#include "menu.h"

#include <stdbool.h>

#include <graphx.h>
#include <keypadc.h>

static void draw_menu_title(void) {
    char title[12] = "Rubik's Race";

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
    switch (menu_selection_idx) {
        case MENU_NEW_GAME:
            gfx_SetColor(2);
            gfx_Rectangle_NoClip(96 - 4, 80 - 2, 8 * 2 * 8 - 4, 8 * 2 + 4);
            break;
        case MENU_SET_SEED:
            gfx_SetColor(4);
            gfx_Rectangle_NoClip(96 - 4, 120 - 2, 8 * 2 * 8 - 4, 8 * 2 + 4);
            break;
        case MENU_PROFILES:
            gfx_SetColor(6);
            gfx_Rectangle_NoClip(96 - 4, 160 - 2, 8 * 2 * 8 - 4, 8 * 2 + 4);
            break;
        default:
            break;
    }
}

static void draw_menu_options(void) {
    gfx_SetTextFGColor(2);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("New Game", 96, 80);

    gfx_SetTextFGColor(4);
    gfx_PrintStringXY("Set Seed", 96, 120);

    gfx_SetTextFGColor(6);
    gfx_PrintStringXY("Profiles", 96, 160);
}

int run_main_menu(void) {
    uint8_t menu_selection_idx = 0;
    const uint8_t main_menu_len = 3;
    kb_key_t previous_arrows = 0;

    // Carryover from entering the app
    {
            gfx_SetDrawBuffer();
            gfx_ZeroScreen();

            draw_menu_title();
            draw_menu_selection(menu_selection_idx);
            draw_menu_options();

            gfx_SwapDraw();

            while (kb_Data[1] || kb_Data[6]) {
                kb_Scan();
            }
    }

    while (true) {
        gfx_SetDrawBuffer();
        gfx_ZeroScreen();

        draw_menu_title();
        draw_menu_selection(menu_selection_idx);
        draw_menu_options();

        gfx_SwapDraw();

        kb_Scan();
        if (kb_Data[6] == kb_Clear) {
            return -1;
        }

        kb_key_t arrows = kb_Data[7];
        if (kb_Data[6] == kb_Enter || kb_Data[1] == kb_2nd) {
            return menu_selection_idx;
        }

        if (arrows == previous_arrows) {
            continue;
        }

        previous_arrows = arrows;

        if (arrows == kb_Down && menu_selection_idx < main_menu_len - 1) {
            menu_selection_idx++;
        } else if (arrows == kb_Up && menu_selection_idx > 0) {
            menu_selection_idx--;
        }

        
    }
}
