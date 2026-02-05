#ifndef __MENUS_H
#define __MENUS_H

enum {
  MENU_MAIN_APP,
  MENU_MAIN_GAMES,
  MENU_MAIN_ABOUT,
};

enum {
  MENU_ABOUT_FIRMWARE,
  MENU_ABOUT_MODEL,
  MENU_ABOUT_CREDITS,
};

enum {
  MENU_GAMES_TETRIS,
  MENU_GAMES_DINO,
  MENU_GAMES_SNAKE,
};

enum {
  MENU_APP_NAME,
  MENU_APP_MACROS,
  MENU_APP_SCREEN_SAVER,
  MENU_APP_HIDDEN,
};

enum {
  MENU_OS_MACRO_LINUX,
  MENU_OS_MACRO_MACOS,
  MENU_OS_MACRO_WINDOWS,
};

enum {
  MENU_MACRO_IFCONFIG,
  MENU_MACRO_RICK
};


void menuApps(void);
void menuAbout(void);
void menuMain(void);
void menuGames(void);
void showVacaAnimation(void);
void showSplashScreen(void);
void menuListMacros(void);
void menuOSMacros(void);
#endif