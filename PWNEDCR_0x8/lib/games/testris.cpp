/* Code by https://www.hackster.io/PCBX_Official/building-a-tetris-game-with-arduino-and-oled-2d48e8
*/

#include <screen.h>
#include <joystick.h>
#include <Adafruit_SH110X.h>

const int gridWidth = 20;
const int gridHeight = 30;
const int blockPixelSize = 4;

// Tetris shapes in a 4x4 grid
const byte shapes[7][4][4] = {
// I
{{0, 1, 0, 0},
{0, 1, 0, 0},
{0, 1, 0, 0},
{0, 1, 0, 0}},
// J
{{0, 0, 0, 0},
{1, 1, 1, 0},
{0, 0, 1, 0},
{0, 0, 0, 0}},
// L
{{0, 0, 0, 0},
{1, 1, 1, 0},
{1, 0, 0, 0},
{0, 0, 0, 0}},
// O
{{0, 0, 0, 0},
{0, 1, 1, 0},
{0, 1, 1, 0},
{0, 0, 0, 0}},
// S
{{0, 0, 0, 0},
{0, 1, 1, 0},
{1, 1, 0, 0},
{0, 0, 0, 0}},
// T
{{0, 0, 0, 0},
{1, 1, 1, 0},
{0, 1, 0, 0},
{0, 0, 0, 0}},
// Z
{{0, 0, 0, 0},
{1, 1, 0, 0},
{0, 1, 1, 0},
{0, 0, 0, 0}}
};

bool grid[gridHeight][gridWidth] = {false};

int currentX = 0;
int currentY = 0;
int currentShape = 0;
int rotation = 0;

unsigned long lastFallTime = 0;
const unsigned long fallSpeed = 500;

static bool running_game = false;

Adafruit_SH1106G dplay = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static void rotateShape(int shape, int rot, byte output[4][4]);
static void newShape(void);
static void drawFrame(void);
static bool isGameOver(void);
static void drawGrid(void);
static void moveShapeDown(void);
static void newShape(void);
static void checkLines(void);
static void fixShape(void);
static void handleInput(void);
static bool checkCollision(int x, int y, int shape, int rot);
static void drawShape(int x, int y, int shape, int rot);

static void tetrisGameShowOver(void){
  screenClear();
  screenDisplayTextBig("Game Over", 0, 32);
  screenDisplay();
  delay(2000);
}

static void tetrisCleanExit(void){
  dplay = getDisplay();
  dplay.clearDisplay();
  dplay.display();
  screenClear();
  screenDisplay();
  joystick_register_game_callback(NULL);
  delay(300);
}

void tetris_game_control(uint8_t direction){
  switch (direction){
    case BUTTON_A: {
      int newRotation = (rotation + 1) % 4;
      if (!checkCollision(currentX, currentY, currentShape, newRotation)) {
        rotation = newRotation;
      }
      break;
    }
    case BUTTON_B:{
      running_game = false;
      tetrisCleanExit();
      memset(grid, false, sizeof(grid));
      joystick_register_game_callback(NULL);
      break;
    }
    case BUTTON_JOYSTICK_UP:
      break;
    case BUTTON_JOYSTICK_DOWN:
      moveShapeDown();
      break;
    case BUTTON_JOYSTICK_LEFT:
      if (!checkCollision(currentX - 1, currentY, currentShape, rotation)) {
        currentX--;
      }
      break;
    case BUTTON_JOYSTICK_RIGHT:
      if (!checkCollision(currentX + 1, currentY, currentShape, rotation)) {
        currentX++;
      }
      break;
    default:
      break;
  }
}

void tetrisSetup(void) {
  dplay = getDisplay();
  joystick_register_game_callback(tetris_game_control);
  newShape();
}

static void tetrisWorker(void){
   if (isGameOver()) {
    tetrisGameShowOver();
    memset(grid, false, sizeof(grid));
    running_game = false;
    joystick_register_game_callback(NULL);
    tetrisCleanExit();
    delay(500);
    return;
  }

  handleInput(); // Handle user input

  if (millis() - lastFallTime >= fallSpeed) {
    lastFallTime = millis();
    moveShapeDown();
  }

  drawFrame();
}

void tetrisRun(void) {
  running_game = true;
  while (running_game){
    tetrisWorker();
  }
}

static void drawFrame(void) {
  dplay.clearDisplay();
  drawGrid();
  drawShape(currentX, currentY, currentShape, rotation);
  dplay.display();
}

static void drawShape(int x, int y, int shape, int rot) {
  byte rotatedShape[4][4];

  rotateShape(shape, rot, rotatedShape);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (rotatedShape[i][j]) {
        int gridX = x + j;
        int gridY = y + i;
        if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
          dplay.fillRect(gridX * blockPixelSize, gridY * blockPixelSize, blockPixelSize, blockPixelSize, SH110X_WHITE);
        }
      }
    }
  }
}

static void drawGrid(void) {
  for (int y = 0; y < gridHeight; y++) {
    for (int x = 0; x < gridWidth; x++) {
      if (grid[y][x]) {
        dplay.fillRect(x * blockPixelSize, y * blockPixelSize, blockPixelSize, blockPixelSize, SH110X_WHITE);
      }
    }
  }
}

static void moveShapeDown(void) {
  if (!checkCollision(currentX, currentY + 1, currentShape, rotation)) {
    currentY++;
  } else {
    fixShape();
    checkLines();
    newShape();
  }
}

static bool checkCollision(int x, int y, int shape, int rot) {
  byte rotatedShape[4][4];
  rotateShape(shape, rot, rotatedShape);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (rotatedShape[i][j]) {
        int gridX = x + j;
        int gridY = y + i;

        if (gridX < 0 || gridX >= gridWidth || gridY >= gridHeight || grid[gridY][gridX]) {
          return true;
        }
      }
    }
  }
  return false;
}

static void fixShape(void) {
  byte rotatedShape[4][4];
  rotateShape(currentShape, rotation, rotatedShape);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (rotatedShape[i][j]) {
        int gridX = currentX + j;
        int gridY = currentY + i;
        if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
          grid[gridY][gridX] = true;
        }
      }
    }
  }
}

static void newShape(void) {
  currentX = gridWidth / 2 - 2;
  currentY = 0;
  currentShape = random(0, 7);
  rotation = 0;
}

static void handleInput(void) {
  joystick_x_read_action();
  joystick_y_read_action();

  if(joystick_button_a_pressed()){
    tetris_game_control(BUTTON_A);
  }
  if(joystick_button_b_pressed()){
    tetris_game_control(BUTTON_B);
  }

  delay(100); // Simple debounce
}

static void checkLines(void) {
  for (int y = gridHeight - 1; y >= 0; y--) {
    bool full = true;
    for (int x = 0; x < gridWidth; x++) {
      if (!grid[y][x]) {
        full = false;
        break;
      }
    }
    if (full) {
      for (int dy = y; dy > 0; dy--) {
        for (int x = 0; x < gridWidth; x++) {
          grid[dy][x] = grid[dy - 1][x];
        }
      }
      for (int x = 0; x < gridWidth; x++) {
        grid[0][x] = false;
      }
      y++; // Recheck this line in the next iteration
    }
  }
}

static bool isGameOver(void) {
  for (int x = 0; x < gridWidth; x++) {
    if (grid[0][x]) {
      return true;
    }
  }
  return false;
}

static void rotateShape(int shape, int rot, byte output[4][4]) {
  // Clear output shape array initially
  memset(output, 0, 16);

  // Perform rotations and store into output
  // Basic matrix rotation - 90 degrees clockwise:
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (rot == 1) {
        output[i][j] = shapes[shape][3-j][i];
      } else if (rot == 2) {
        output[i][j] = shapes[shape][3-i][3-j];
          } else if (rot == 3) {
    output[i][j] = shapes[shape][j][3-i];
  } else {
    output[i][j] = shapes[shape][i][j];
  }
}
}
}