#include <screen.h>
#include <joystick.h>
#include <Adafruit_SH110X.h>

// made by ultramegabombastiucfuze on https://projecthub.arduino.cc/

#define BUTTON_PIN 3  // button pin 

// Game state variables
bool isJumping = false;
bool gameOver = false;
int dinoY = 40;
int velocity = 0;
const int gravity = 2;
const int groundY = 40;
int cactusX1 = 128;
int cactusX2 = 180;
int gameSpeed = 3;
unsigned long lastSpeedIncrease = 0;
unsigned long lastFrame = 0;
unsigned long score = 0;

// Button debounce state
bool buttonPressed = false;
static bool running_game = false;
static void resetGame(void);

Adafruit_SH1106G diplay = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static void dino_game_control(uint8_t direction){
  switch (direction){
    case BUTTON_A: {
      if (!buttonPressed && dinoY == groundY) {
        buttonPressed = true;
        isJumping = true;
        velocity = -13;
        buttonPressed = false;
      }
      break;
    }
    case BUTTON_B:{
      running_game = false;
      diplay = getDisplay();
      diplay.clearDisplay();
      diplay.display();
      screenClear();
      screenDisplay();
      joystick_register_game_callback(NULL);
      delay(300);
      break;
    }
    case BUTTON_JOYSTICK_UP:
    case BUTTON_JOYSTICK_DOWN:
    case BUTTON_JOYSTICK_LEFT:
    case BUTTON_JOYSTICK_RIGHT:
    default:
      break;
  }
}

static void dinoGameShowOver(void){
  screenClear();
  screenDisplayTextBig("Game Over", 0, 32);
  screenDisplay();
  delay(2000);
}

static void dinoWorker(void) {
  unsigned long currentTime = millis();
  if (currentTime - lastFrame < 50) return;
  lastFrame = currentTime;

  if (gameOver) {
    dinoGameShowOver();
    running_game = false;
    diplay = getDisplay();
    diplay.clearDisplay();
    diplay.display();
    screenClear();
    screenDisplay();
    joystick_register_game_callback(NULL);
    delay(300);
    return;
  }

  if(joystick_button_a_pressed()){
    dino_game_control(BUTTON_A);
  }
  if(joystick_button_b_pressed()){
    dino_game_control(BUTTON_B);
  }

  // --- PHYSICS ---
  if (isJumping) {
    dinoY += velocity;
    velocity += gravity;
    if (dinoY >= groundY) {
      dinoY = groundY;
      isJumping = false;
    }
  }

  // --- MOVE CACTI ---
  cactusX1 -= gameSpeed;
  cactusX2 -= gameSpeed;

  if (cactusX1 < -10) {
    cactusX1 = 128 + random(0, 40);
    if (random(0, 10) > 6) cactusX2 = cactusX1 + random(15, 30);
  }
  if (cactusX2 < -10) {
    cactusX2 = 128 + random(30, 60);
  }

  // --- SPEED UP ---
  if (currentTime - lastSpeedIncrease > 3000) {
    gameSpeed++;
    lastSpeedIncrease = currentTime;
  }

  // --- SCORE ---
  score++;

  // --- DRAW SCENE ---
  diplay.clearDisplay();
  diplay.drawBitmap(10, dinoY, epd_bitmap_CalacaWhite, 16, 16, SH110X_WHITE);
  // diplay.fillRect(10, dinoY, 10, 10, SH110X_WHITE);        // Dino
  diplay.fillRect(cactusX1, groundY, 10, 15, SH110X_WHITE); // Cactus 1
  diplay.fillRect(cactusX2, groundY, 10, 15, SH110X_WHITE); // Cactus 2
  diplay.drawLine(0, 58, 128, 58, SH110X_WHITE);            // Ground
  diplay.setTextSize(1);
  diplay.setTextColor(SH110X_WHITE);
  diplay.setCursor(0, 0);
  diplay.print("Score: ");
  diplay.print(score / 10);
  diplay.display();

  // --- COLLISION DETECTION ---
  bool hitCactus1 = (cactusX1 < 20 && cactusX1 > 5 && dinoY == groundY);
  bool hitCactus2 = (cactusX2 < 20 && cactusX2 > 5 && dinoY == groundY);

  if (hitCactus1 || hitCactus2) {
    gameOver = true;
    screenClear();
    screenDisplayTextBig("Game Over", 0, 32);
    screenDisplay();
    delay(2000);
  }
}

// --- RESTART GAME FUNCTION ---
static void resetGame(void) {
  isJumping = false;
  dinoY = groundY;
  velocity = 0;
  cactusX1 = 128;
  cactusX2 = 180;
  gameSpeed = 3;
  score = 0;
  gameOver = false;
  lastSpeedIncrease = millis();
  lastFrame = millis();
  diplay.clearDisplay();
  diplay.display();
}

void dinoSetup(void) {
  diplay = getDisplay();
  diplay.clearDisplay();
  diplay.display();
  randomSeed(analogRead(0));  // Randomize cactus spawn
  joystick_register_game_callback(dino_game_control);
  resetGame();
}

void dinoRun(void){
  running_game = true;
  while (running_game){
    dinoWorker();
  }
}