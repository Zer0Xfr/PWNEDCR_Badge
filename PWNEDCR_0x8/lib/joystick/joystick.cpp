#include <joystick.h>

Button buttonA(PIN_BUTTON_A);
Button buttonB(PIN_BUTTON_B);

static uint8_t joystick_x_state = 0;
static uint8_t joystick_y_state = 0;

static joystick_action_cb joystick_cb = NULL;
static joystick_action_cb joystick_game_cb = NULL;

static void joystick_read_y(int16_t* y) {
  *y = (analogRead(PIN_JOYSTICK_Y) / 100);
}

static void joystick_read_x(int16_t* x) {
  *x = (analogRead(PIN_JOYSTICK_X) / 100);
}

static void joystick_button_action(uint8_t button){
  if (joystick_game_cb){
    joystick_game_cb(button);
    return;
  }
  if (joystick_cb && !joystick_game_cb){
    joystick_cb(button);
  }
}

void joystick_x_read_action(void){
  int16_t x;
  joystick_read_x(&x);
  if (x < JOYSTICK_DEADZONE_X - JOYSTICK_DEADZONE_TOLERANCE) {
    if (joystick_x_state == 1){ return;}
    joystick_button_action(BUTTON_JOYSTICK_RIGHT);
    joystick_x_state = 1;
  } else if (x > JOYSTICK_DEADZONE_X + JOYSTICK_DEADZONE_TOLERANCE) {
    if (joystick_x_state == 2){ return;}
    joystick_button_action(BUTTON_JOYSTICK_LEFT);
    joystick_x_state = 2;
  }else{
    joystick_x_state = 0;
  }
}

void joystick_y_read_action(void){
  int16_t y;
  joystick_read_y(&y);
  if (y < JOYSTICK_DEADZONE_Y - JOYSTICK_DEADZONE_TOLERANCE) {
    if (joystick_y_state == 1){ return;}
    joystick_button_action(BUTTON_JOYSTICK_UP);
    joystick_y_state = 1;
  } else if (y > JOYSTICK_DEADZONE_Y + JOYSTICK_DEADZONE_TOLERANCE) {
    if (joystick_y_state == 2){ return;}
    joystick_button_action(BUTTON_JOYSTICK_DOWN);
    joystick_y_state = 2;
  }else{
    joystick_y_state = 0;
  }
}

void joystick_register_callback(joystick_action_cb cb){
  joystick_cb = cb;
}

void joystick_register_game_callback(joystick_action_cb cb){
  joystick_game_cb = cb;
}


void joystick_init(void){
  pinMode(PIN_BUTTON_A, INPUT_PULLUP);
  pinMode(PIN_BUTTON_B, INPUT_PULLUP);

  buttonA.begin();
  buttonB.begin();
}

bool joystick_button_a_pressed(void){
  return buttonA.pressed();
}
bool joystick_button_b_pressed(void){
  return buttonB.pressed();
}