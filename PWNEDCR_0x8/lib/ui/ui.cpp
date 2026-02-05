#include <ui.h>
#include <joystick.h>

#define MAX_OPTION_LEN 10
#define MAX_OPTIONS_NUM 6

static general_menu_t menu_ctx_storage;
static general_menu_t* menu_ctx = &menu_ctx_storage;
static bitmap_t bitmap_circle = {epd_bitmap_circle, 64, 64};

static uint8_t konami_code[] = {
    BUTTON_JOYSTICK_UP, BUTTON_JOYSTICK_UP, BUTTON_JOYSTICK_DOWN,
    BUTTON_JOYSTICK_DOWN, BUTTON_JOYSTICK_LEFT, BUTTON_JOYSTICK_RIGHT,
    BUTTON_JOYSTICK_LEFT, BUTTON_JOYSTICK_RIGHT};
static uint8_t konami_code_progress = 0;
static uint8_t user_input_sequence[sizeof(konami_code)] = {0};

static void list_submenu_options() {
  general_menu_t* ctx = menu_ctx;
  if (!ctx || !ctx->options || ctx->options_count == 0) {
    screenClear();
    screenDisplayText("Error: No options", 0, 0);
    screenDisplay();
    return;
  }
  static uint8_t items_offset = 0;
  items_offset = MAX(ctx->selected_option - MAX_OPTIONS_NUM + 2, items_offset);
  items_offset =
      MIN(MAX(ctx->options_count - MAX_OPTIONS_NUM + 2, 0), items_offset);
  items_offset = MIN(ctx->selected_option, items_offset);
  screenClear();

  if(ctx->banner){
    screenDisplayTextCenter(ctx->banner, 0);
  }

  char* str = (char*) malloc(MAX_OPTION_LEN + 1);
  if (!str) {
    screenDisplay();
    return;
  }
  for (uint8_t i = 0; i < (MIN(ctx->options_count, MAX_OPTIONS_NUM - 1)); i++) {
    uint8_t idx = i + items_offset;
    if (idx >= ctx->options_count || !ctx->options[idx]) {
      printf("Invalid option at index %d\n", idx);
      continue;
    }
    bool is_selected = idx == ctx->selected_option;
    snprintf(str, MAX_OPTION_LEN + 1, "%s", ctx->options[idx]);
    const uint8_t row_height = 16;
    uint16_t y_pos = (i + 2) * row_height;

    if (is_selected) {
      snprintf(str, MAX_OPTION_LEN + 1, ">%s", ctx->options[idx]);
      screenDisplayTextCenter(str, y_pos);
    } else {
      screenDisplayTextCenter(str, y_pos);
    }
  }
  screenDisplay();
  free(str);
}

void general_menu(general_menu_t menu) {
  memset(menu_ctx, 0, sizeof(*menu_ctx));
  *menu_ctx = menu;
  list_submenu_options();
}

void general_check_code(uint8_t input){
  user_input_sequence[konami_code_progress] = input;
  if(input == konami_code[konami_code_progress]){
    konami_code_progress++;
    if(konami_code_progress >= sizeof(konami_code)){
      // Konami code entered successfully
      screenClear();
      screenDisplayTextCenter("KONAMI", 16);
      screenDisplayTextCenter("UNLOCKED", 32);
      screenShowBitmap(&bitmap_circle, 0, 64);
      screenDisplay();
      delay(3000);
      konami_code_progress = 0;
      list_submenu_options();
    }
  }else{
    konami_code_progress = 0;
  }
}

void general_menu_navigate(uint8_t direction){
  general_check_code(direction);
  switch (direction){
    case BUTTON_A: {
      submenu_selection_handler_t select_cb = menu_ctx->select_cb;
      if (select_cb) {
        select_cb(menu_ctx->selected_option);
      }
      break;
    }
    case BUTTON_B:{      
      if (menu_ctx->exit_cb){
        menu_ctx->exit_cb();
      }
      break;
    }
    case BUTTON_JOYSTICK_UP:
      Serial.println("Up");
      if (menu_ctx->selected_option > 0) {
        menu_ctx->selected_option--;
        list_submenu_options();
      }
      break;
    case BUTTON_JOYSTICK_DOWN:
      Serial.println("DOWN");
      if (menu_ctx->selected_option < menu_ctx->options_count - 1) {
        menu_ctx->selected_option++;
        list_submenu_options();
      }
      break;
    case BUTTON_JOYSTICK_LEFT:
      Serial.println("LEFT");
      break;
    case BUTTON_JOYSTICK_RIGHT:
      Serial.println("RIGHT");
      break;
    default:
      break;
  }
}