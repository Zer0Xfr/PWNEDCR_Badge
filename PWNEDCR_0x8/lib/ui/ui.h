#ifndef __UI_H
#define __UI_H

#include <screen.h>

typedef void (*submenu_selection_handler_t)(uint8_t);
typedef void (*exit_handler_t)(void);

typedef struct {
  char** options;
  const char* banner;
  uint8_t options_count;
  uint8_t selected_option;
  submenu_selection_handler_t select_cb;
  exit_handler_t exit_cb;
} general_menu_t;

void general_menu(general_menu_t submenu);
void general_menu_navigate(uint8_t direction);
#endif