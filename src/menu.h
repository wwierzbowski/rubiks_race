#ifndef MENU_H
#define MENU_H

#include <stdint.h>

enum MenuSelection {
    MENU_NEW_GAME = 0,
    MENU_SET_SEED = 1,
    MENU_PROFILES = 2
};

void init_main_menu(void);
int run_main_menu(void);

#endif
