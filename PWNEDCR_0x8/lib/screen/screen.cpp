#include <SPI.h>
#include <Wire.h>
#include <screen.h>

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_SH1106G getDisplay(void){
  return display;
}

bool screenBegin(void) {
  // display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  return display.begin(SCREEN_ADDRESS, true);
}

void screenConfigure(void) {
  display.setRotation(1);
}

void screenClear(void) {
  display.clearDisplay();
}

void screenDisplay(void) {
  display.display();
}

void screenShowBitmap(bitmap_t* bitmap, uint16_t x, uint16_t y) {
  display.drawBitmap(x, y, bitmap->bitmap, bitmap->width, bitmap->height,
                     SH110X_WHITE);
}

void screenGetCenterCoordsFromBitmap(bitmap_t* bitmap,
                                     screen_coords_t* coords) {
  coords->x = (SCREEN_WIDTH - bitmap->width) / 2;
  coords->y = (SCREEN_HEIGHT - bitmap->height) / 2;
}

void screenDisplayText(const char* text, uint16_t x, uint16_t y) {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(x, y);
  display.println(text);
}

void screenDisplayTextCenter(const char* text, uint16_t y) {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  uint16_t x = (SCREEN_WIDTH - (strlen(text) * 6)) / 2;
  if(display.getRotation() == 1 || display.getRotation() == 3){
    x = (SCREEN_HEIGHT - (strlen(text) * 6)) / 2;
  }
  
  display.setCursor(x, y);
  display.println(text);
}

void screenDisplayTextBig(const char* text, uint16_t x, uint16_t y) {
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(x, y);
  display.println(text);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SH110X_WHITE);
    display.display();
    delay(1);
  }
}