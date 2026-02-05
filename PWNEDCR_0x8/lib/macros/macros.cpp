#include <macros.h>

void macrosBegin(void){
  Keyboard.begin();
}

void macroRunTerminalWindows(const char* cmd){
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.print("cmd");
  Keyboard.write(KEY_RETURN);
  delay(1000);
  Keyboard.print(cmd);
}
void macroRunTerminalLinux(const char* cmd){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.print(cmd);
}

void macroRunTerminalMacos(const char* cmd){
  Keyboard.begin();
  Keyboard.press(KEY_LEFT_GUI);  // CMD + SPACE
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.print("terminal");
  Keyboard.write(KEY_RETURN);
  delay(1500);
  Keyboard.print(cmd);
  Keyboard.end();
}