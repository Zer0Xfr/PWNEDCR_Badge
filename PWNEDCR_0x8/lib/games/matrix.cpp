#include <screen.h>
#include <joystick.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G maxdiplay = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define COLS (SCREEN_WIDTH / 6)  // aprox. 6px por carácter
#define MAX_SPEED 3

// Datos de las columnas
static int y_pos[COLS];
static int speed[COLS];

static bool running_game = false;

static void matrix_game_control(uint8_t direction){
  switch (direction){
    case BUTTON_B:{
      running_game = false;
      maxdiplay = getDisplay();
      maxdiplay.clearDisplay();
      maxdiplay.display();
      screenClear();
      screenDisplay();
      joystick_register_game_callback(NULL);
      delay(300);
      break;
    }
    case BUTTON_A:
    case BUTTON_JOYSTICK_UP:
    case BUTTON_JOYSTICK_DOWN:
    case BUTTON_JOYSTICK_LEFT:
    case BUTTON_JOYSTICK_RIGHT:
    default:
      break;
  }
}

void matrixSetup(void) {
  maxdiplay = getDisplay();
  maxdiplay.clearDisplay();
  maxdiplay.display();
  for (int i = 0; i < COLS; i++) {
    y_pos[i] = random(-SCREEN_HEIGHT, 0);
    speed[i] = random(1, MAX_SPEED + 1);
  }
}

static void matrixWorker(void) {
  maxdiplay.clearDisplay();

  for (int i = 0; i < COLS; i++) {
    int y = y_pos[i];

    for (int j = 0; j < 16; j++) {
      int char_y = y - j * 8;
      if (char_y >= 0 && char_y < SCREEN_HEIGHT) {
        maxdiplay.setCursor(i * 6, char_y);
        char c = 33 + random(94);
        maxdiplay.write(c);
      }
    }

    // Actualiza posición
    y_pos[i] += speed[i];
    if (y_pos[i] > SCREEN_HEIGHT + 20) {
      y_pos[i] = random(-SCREEN_HEIGHT, 0);
      speed[i] = random(1, MAX_SPEED + 1);
    }
  }
  if(joystick_button_b_pressed()){
    matrix_game_control(BUTTON_B);
  }

  maxdiplay.display();
  delay(40);
}

void matrixRun(void){
  running_game = true;
  while (running_game){
    matrixWorker();
  }
}

