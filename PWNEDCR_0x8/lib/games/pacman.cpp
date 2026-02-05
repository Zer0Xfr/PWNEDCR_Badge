#include <screen.h>
#include <joystick.h>
#include <Adafruit_SH110X.h>

#define SNAKE_BLOCK 2 // Size of each snake block
#define MAX_SNAKE_LENGTH 50

Adafruit_SH1106G pacdisplay = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static bool running_game = false;
static int snakeX = 32;
static int snakeY = 32;
static int dirX = 0;
static int dirY = -SNAKE_BLOCK;
static int foodX, foodY;
static int snakeBody[MAX_SNAKE_LENGTH][2];
static int snakeLength = 1;
static int gameSpeed = 100;

static void moveSnake(void);
static void increaseSnake(void);
static bool checkCollision(void);
static void placeFood(void);
static bool checkFoodCollision(void);
static void drawScreen(void);
static void displayGameOver(void);

static void resetGame(void){
  snakeLength = 1;
  snakeX = 32;
  snakeY = 32;
  dirX = 0;
  dirY = -SNAKE_BLOCK;
  gameSpeed = 100;

  snakeBody[0][0] = snakeX;
  snakeBody[0][1] = snakeY;

  placeFood();
}

static void snake_game_control(uint8_t direction){
  switch (direction){
    case BUTTON_A:
      break;
    case BUTTON_B:{
      running_game = false;
      pacdisplay = getDisplay();
      pacdisplay.clearDisplay();
      pacdisplay.display();
      screenClear();
      screenDisplay();
      joystick_register_game_callback(NULL);
      delay(300);
      break;
    }
    case BUTTON_JOYSTICK_UP:
      if (dirY != SNAKE_BLOCK) {
        dirX = 0; dirY = -SNAKE_BLOCK;
      }
      break;
    case BUTTON_JOYSTICK_DOWN:
      if (dirY != -SNAKE_BLOCK) {
        dirX = 0; dirY = SNAKE_BLOCK;
      }
      break;
    case BUTTON_JOYSTICK_LEFT:
      if (dirX != SNAKE_BLOCK) {
        dirX = -SNAKE_BLOCK; dirY = 0;
      }
      break;
    case BUTTON_JOYSTICK_RIGHT:
      if (dirX != -SNAKE_BLOCK) {
        dirX = SNAKE_BLOCK; dirY = 0;
      }
      break;
    default:
      break;
  }
}

static void moveSnake(void) {
  int prevX = snakeX, prevY = snakeY;
  snakeX += dirX;
  snakeY += dirY;

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeBody[i][0] = snakeBody[i - 1][0];
    snakeBody[i][1] = snakeBody[i - 1][1];
  }

  snakeBody[0][0] = snakeX;
  snakeBody[0][1] = snakeY;
}

static void increaseSnake(void) {
  if (snakeLength < MAX_SNAKE_LENGTH) {
    snakeLength++;
  }
}

void snakeSetup(void) {
  pacdisplay = getDisplay();
  pacdisplay.clearDisplay();
  pacdisplay.display();
  joystick_register_game_callback(snake_game_control);

  snakeBody[0][0] = snakeX;
  snakeBody[0][1] = snakeY;

  resetGame();
}

static bool checkCollision(void) {
  if (snakeX < 0 || snakeX >= SCREEN_HEIGHT || snakeY < 0 || snakeY >= SCREEN_WIDTH) {
    return true;
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snakeX == snakeBody[i][0] && snakeY == snakeBody[i][1]) {
      return true;
    }
  }

  return false;
}

static void placeFood(void) {
  do {
    foodX = (random(0, SCREEN_HEIGHT / SNAKE_BLOCK) * SNAKE_BLOCK);
    foodY = (random(0, SCREEN_WIDTH / SNAKE_BLOCK) * SNAKE_BLOCK);
  } while (checkFoodCollision());
}

static bool checkFoodCollision(void) {
  for (int i = 0; i < snakeLength; i++) {
    if (foodX == snakeBody[i][0] && foodY == snakeBody[i][1]) {
      return true;
    }
  }
  return false;
}

static void drawScreen(void) {
  pacdisplay.clearDisplay();

  for (int i = 0; i < snakeLength; i++) {
    pacdisplay.fillRect(snakeBody[i][0], snakeBody[i][1], SNAKE_BLOCK, SNAKE_BLOCK, SH110X_WHITE);
  }

  pacdisplay.fillRect(foodX, foodY, SNAKE_BLOCK, SNAKE_BLOCK, SH110X_WHITE);

  pacdisplay.display();
}

static void displayGameOver(void) {
  screenClear();
  screenDisplayTextBig("Game Over", 0, 32);
  screenDisplay();
  delay(2000);
}

void snakeWorker() {
  joystick_x_read_action();
  joystick_y_read_action();

  if(joystick_button_a_pressed()){
    snake_game_control(BUTTON_A);
  }
  if(joystick_button_b_pressed()){
    snake_game_control(BUTTON_B);
  }
  
  moveSnake();

  if (snakeX == foodX && snakeY == foodY) {
    increaseSnake();
    placeFood();
    gameSpeed = max(50, gameSpeed - 10);
  }

  if (checkCollision()) {
    displayGameOver();
    running_game = false;
    pacdisplay = getDisplay();
    pacdisplay.clearDisplay();
    pacdisplay.display();
    screenClear();
    screenDisplay();
    joystick_register_game_callback(NULL);
    delay(300);
    return;
  }

  drawScreen();

  delay(gameSpeed);
}

void snakeRun(void){
  running_game = true;
  while (running_game){
    snakeWorker();
  }
}