#ifndef __SCREEN_H
#define __SCREEN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>
#include <bitmaps.h>
#include <pins.h>

typedef struct {
  uint16_t x;
  uint16_t y;
} screen_coords_t;

Adafruit_SH1106G getDisplay();
bool screenBegin(void);
void screenConfigure(void);
void screenClear(void);
void screenDisplay(void);
void screenShowBitmap(bitmap_t* bitmap, uint16_t x, uint16_t y);
void screenGetCenterCoordsFromBitmap(bitmap_t* bitmap, screen_coords_t* coords);
void screenDisplayText(const char* text, uint16_t x, uint16_t y);
void screenDisplayTextBig(const char* text, uint16_t x, uint16_t y);
void screenDisplayTextCenter(const char* text, uint16_t y);


void testdrawroundrect(void);
#endif