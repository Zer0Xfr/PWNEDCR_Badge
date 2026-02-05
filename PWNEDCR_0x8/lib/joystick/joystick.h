#ifndef __JOYSTICK_H
#define __JOYSTICK_H
#include <Arduino.h>
#include <pins.h>
#include <Button.h>

#define MAX_JOUYSTICK_VALUE 1023
#define MIN_JOUYSTICK_VALUE 0
#define JOYSTICK_DEADZONE_Y 5
#define JOYSTICK_DEADZONE_X 5
#define JOYSTICK_DEADZONE_TOLERANCE 2

typedef void (*joystick_action_cb)(uint8_t);

enum {
  BUTTON_A,
  BUTTON_B,
  BUTTON_JOYSTICK_UP,
  BUTTON_JOYSTICK_DOWN,
  BUTTON_JOYSTICK_LEFT,
  BUTTON_JOYSTICK_RIGHT,
};

void joystick_init(void);
void joystick_x_read_action(void);
void joystick_y_read_action(void);
void joystick_register_callback(joystick_action_cb cb);
void joystick_register_game_callback(joystick_action_cb cb);
bool joystick_button_a_pressed(void);
bool joystick_button_b_pressed(void);
#endif