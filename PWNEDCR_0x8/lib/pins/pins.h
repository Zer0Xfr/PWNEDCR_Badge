#ifndef __PINS_H
#define __PINS_H

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET D8      // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

#define PIN_SDA 4
#define PIN_SCL 5

#define PIN_JOYSTICK_X A1 //DEV: A0
#define PIN_JOYSTICK_Y A2 //DEV: A1

#define PIN_BUTTON_A A3 // DEV: 14
#define PIN_BUTTON_B A0 // DEV: 15

#endif