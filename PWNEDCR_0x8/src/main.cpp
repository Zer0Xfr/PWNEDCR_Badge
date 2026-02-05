#include <menus.h>
#include <screen.h>
#include <Button.h>
#include <joystick.h>
#include <pins.h>
#include <ui.h>
#include <cmd.h>
#include <flags.h>
#include <EEPROM.h>
#include <macros.h>

static bool splash_displayed = true;
static unsigned long splash_start = 0;
static unsigned long last_blink = 0;
static bool splash_visible = false;

const unsigned long splash_duration = 500000;
const unsigned long blink_interval = 800;
const unsigned long blink_pause = 500;

void setup() {
  Serial.begin(115200);

  EEPROM.begin(64);
  macrosBegin();

  joystick_init();
  Serial.println(FLAG_SERIAL_INIT);

  if (!screenBegin()) {
    for (;;) {
      Serial.println("SSD1306 allocation failed");
      delay(1000);
    }
  }

  screenConfigure();
  joystick_register_callback(general_menu_navigate);

  showSplashScreen();
  delay(5000);
  showVacaAnimation();

  splash_start = millis();
  last_blink = millis();
}

void setup1() {
  cmdBegin();
}

static void showInitialSplashScreen() {
  unsigned long now = millis();

  if (splash_visible && (now - last_blink > blink_interval)) {
    screenClear();
    screenDisplay();
    splash_visible = false;
    last_blink = now;
  } else if (!splash_visible && (now - last_blink > blink_pause)) {
    screenClear();
    screenDisplayTextCenter("Pwned x8", 0);
    screenDisplayTextCenter("Insert", 32);
    screenDisplayTextCenter("Coin", 48);
    screenDisplayTextCenter("2025", 112);
    screenDisplay();
    splash_visible = true;
    last_blink = now;
  }

  if (joystick_button_a_pressed()) {
    splash_displayed = false;
    screenClear();
    menuMain();
    return;
  }

  if (now - splash_start > splash_duration) {
    splash_displayed = false;
    screenClear();
    menuMain();
    return;
  }
}

static void controlGeneral() {
  joystick_x_read_action();
  joystick_y_read_action();

  if (joystick_button_a_pressed()) {
    general_menu_navigate(BUTTON_A);
  }
  if (joystick_button_b_pressed()) {
    general_menu_navigate(BUTTON_B);
  }
}

void loop() {
  if (splash_displayed) {
    showInitialSplashScreen();
  } else {
    controlGeneral();
  }
  delay(30);
}

void loop1() {
  cmdRead();
}
