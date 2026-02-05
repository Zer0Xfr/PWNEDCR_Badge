#include <ui.h>
#include <menus.h>
#include <games.h>
#include <joystick.h>
#include <EEPROM.h>
#include <shared.h>
#include <flags.h>
#include <macros.h>

#define FLAG_UNLOCKED_LIMIT 5

static bitmap_t bitmap_pwned = {epd_bitmap_circle, 64, 64};
static bitmap_t bitmap_flag = {epd_bitmap_vaca_flag, 64, 64};
static bitmap_t bitmap_pwned32 = {epd_bitmap_Pwnedbanner, 32, 32};
static char* options_main[] = {"App", "Games", "About"};
static char* options_about[] = {"Firmware", "Model", "Credits"};
static char* options_app[] = {"Name", "Macros", "Saver", ""};
static char* options_games[] = {"Tetris", "Smiley", "Snake"};
static char* options_os_macros[] = {"Linux", "Macos", "Windows"};
static char* options_macros[] = {"IP Conf", "Pwnedcr"};

static uint8_t firmware_counter = 0;
static uint8_t changed_option = 0;
static uint8_t os_selected = MENU_OS_MACRO_LINUX;

static void showAboutSplashScreenFlag(void);
static void showAboutFirmwareVersion(void);
static void showAboutModel(void);
static void showAboutCredits(void);
static void showAppName(void);
static void showHiddenFlag(void);
static void showExecCmd(void);

static const unsigned char* epd_bitmap_allArray[2] = {
	epd_bitmap_Vaca1,
	epd_bitmap_Vaca2
};

static void onMenuGamesSelection(uint8_t option){
  switch (option) {
  case MENU_GAMES_TETRIS:
    tetrisSetup();
    tetrisRun();
    joystick_register_game_callback(NULL);
    break;
  case MENU_GAMES_DINO:
    dinoSetup();
    dinoRun();
    joystick_register_game_callback(NULL);
    break;
  case MENU_GAMES_SNAKE:
    snakeSetup();
    snakeRun();
    joystick_register_game_callback(NULL);
    break;
  default:
    break;
  }
  screenClear();
  screenDisplay();
  delay(100);
  screenClear();
  screenDisplay();
  delay(100);
  menuMain();
}

static void onMenuAppSelection(uint8_t option){
  switch (option) {
  case MENU_APP_NAME:
    showAppName();
    break;
  case MENU_APP_MACROS:
    menuOSMacros();
    break;
  case MENU_APP_SCREEN_SAVER:
    matrixSetup();
    matrixRun();
    joystick_register_game_callback(NULL);
    screenClear();
    screenDisplay();
    delay(100);
    screenClear();
    screenDisplay();
    delay(100);
    menuApps();
    break;
  case MENU_APP_HIDDEN:
    showHiddenFlag();
    break;
  default:
    break;
  }
}

static void onMenuSelection(uint8_t option){
  switch (option) {
  case MENU_MAIN_APP:
    menuApps();
    break;
  case MENU_MAIN_GAMES:
    menuGames();
    break;
  case MENU_MAIN_ABOUT:
    menuAbout();
    break;
  default:
    break;
  }
}

static void onMenuOsMacroSelection(uint8_t option){
  os_selected = option;
  switch (option) {
  case MENU_OS_MACRO_LINUX:
  case MENU_OS_MACRO_MACOS:
  case MENU_OS_MACRO_WINDOWS:
    menuListMacros();
    break;
  default:
    break;
  }
}


static void onMenuListMacroSelection(uint8_t option){
  switch (option) {
  case MENU_MACRO_IFCONFIG:
    if (os_selected == MENU_OS_MACRO_LINUX){
      macroRunTerminalLinux(LINUX_IFCONFIG);
    }else if (os_selected == MENU_OS_MACRO_MACOS){
      macroRunTerminalMacos(MACOS_IFCONFIG);
    }else{
      macroRunTerminalWindows(WINDOWS_IFCONFIG);
    }
    break;
  case MENU_MACRO_RICK:
    char buffer[1024];
    if (os_selected == MENU_OS_MACRO_LINUX){
      sprintf(buffer, "xdg-open %s\n", GLOBAL_RICK_URL);
      macroRunTerminalLinux(buffer);
    }else if (os_selected == MENU_OS_MACRO_MACOS){
      sprintf(buffer, "open %s\n", GLOBAL_RICK_URL);
      macroRunTerminalMacos(buffer);
    }else{
      sprintf(buffer, "start %s\n", GLOBAL_RICK_URL);
      macroRunTerminalWindows(buffer);
    }
    break;
  default:
    break;
  }
  showExecCmd();
  menuListMacros();
}

static void onMenuAboutSelection(uint8_t option){
  switch (option) {
  case MENU_ABOUT_FIRMWARE:
    if (changed_option == 0){
      firmware_counter++;
      if(firmware_counter > FLAG_UNLOCKED_LIMIT){
        showAboutSplashScreenFlag();
        firmware_counter = 0;
        break;
      }
    }else{
      changed_option = 0;
      firmware_counter = 0;
    }
    showAboutFirmwareVersion();
    break;
  case MENU_ABOUT_MODEL:
    changed_option = 1;
    showAboutModel();
    break;
  case MENU_ABOUT_CREDITS:
    changed_option = 1;
    showAboutCredits();
    break;
  default:
    break;
  }
  menuAbout();
}

static void showAboutSplashScreenFlag(void){
  screenClear();
  screenDisplayTextCenter("FLAG", 0);
  screenShowBitmap(&bitmap_flag, 0, 16);
  screenDisplayTextCenter("UNLOCKED", bitmap_flag.height + 32);
  screenDisplay();
  delay(5000);
}

static void showAppName(void){
  screenClear();
  screenDisplayTextCenter("Hello", 0);
  screenDisplayTextCenter("My Name", 16);
  screenDisplayTextCenter("is", 32);
  
  uint16_t signature;
  EEPROM.get(EEPROM_SIGNATURE_ADDR, signature);

  char name[MAX_NAME_LEN + 1] = {0};

  if (signature == SIGNATURE_VALUE) {
    for (int i = 0; i < MAX_NAME_LEN; i++) {
      name[i] = EEPROM.read(EEPROM_NAME_ADDR + i);
      if (name[i] == '\0') break;
    }
    if (isPrintable(name[0])) {
      screenDisplayTextBig(name, 0, 64);
    } else {
      screenDisplayTextBig("Pwned", 0, 64);
    }

  } else {
    screenDisplayTextBig("Pwned", 0, 64);
  }

  screenDisplay();
}

static void showAboutFirmwareVersion(void){
  char version_str[20];
  snprintf(version_str, sizeof(version_str), "v1.2.0");
  screenClear();
  screenDisplayTextCenter("Version", 16);
  screenDisplayTextCenter(version_str, 32);
  screenDisplay();
  delay(2000);
}

static void showAboutModel(void){
  screenClear();
  screenDisplayTextCenter("DEVKITTYIO", 16);
  screenDisplayTextCenter("PWNEDCR", 32);
  screenDisplayTextCenter("0x8", 48);
  screenDisplay();
  delay(2000);
}

static void showAboutCredits(void){
  screenClear();
  screenDisplayTextCenter("DEVKITTYIO", 16);
  screenDisplayTextCenter("PWNEDCR", 32);
  screenDisplayTextCenter("Kev", 48);
  screenDisplayTextCenter("Bitbl4ck", 64);
  screenDisplayTextCenter("Z3r0", 80);
  screenDisplay();
  delay(4000);
}

static void showHiddenFlag(void){
  screenClear();
  screenDisplayTextCenter("Useless", 16);
  screenDisplayTextCenter("Flag", 32);
  screenDisplayTextCenter(FLAG_HIDDEN_SCREEN, 48);
  screenShowBitmap(&bitmap_pwned32, 16, 64);
  screenDisplay();
  delay(2000);
}

static void showExecCmd(void){
  screenClear();
  screenDisplayTextCenter("Writing", 32);
  screenDisplayTextCenter("Command", 48);
  screenDisplay();
  delay(1000);
}

void showVacaAnimation(void){
  for(int j = 0; j < 3; j++){
    for(int i = 0; i < 2; i++){
      screenClear();
      screenDisplay();
      bitmap_t bit = {epd_bitmap_allArray[i], 64, 64};
      screenShowBitmap(&bit, 0, 16);
      screenDisplay();
      delay(800);
      screenClear();
      screenDisplay();
      delay(100);
    }
    delay(100);
  }
}

void menuApps(void){
  general_menu_t games_menu;
  games_menu.options         = options_app;
  games_menu.banner          = "Apps";
  games_menu.options_count   = sizeof(options_app) / sizeof(char*);
  games_menu.selected_option = 0;
  games_menu.select_cb       = onMenuAppSelection;
  games_menu.exit_cb         = menuMain;
  general_menu(games_menu);
}

void menuGames(void){
  general_menu_t games_menu;
  games_menu.options         = options_games;
  games_menu.banner          = "Games";
  games_menu.options_count   = sizeof(options_games) / sizeof(char*);
  games_menu.selected_option = 0;
  games_menu.select_cb       = onMenuGamesSelection;
  games_menu.exit_cb         = menuMain;
  general_menu(games_menu);
}

void menuAbout(void){
  general_menu_t about_menu;
  about_menu.options         = options_about;
  about_menu.banner          = "About";
  about_menu.options_count   = sizeof(options_about) / sizeof(char*);
  about_menu.selected_option = 0;
  about_menu.select_cb       = onMenuAboutSelection;
  about_menu.exit_cb         = menuMain;
  general_menu(about_menu);
}

void menuListMacros(void){
  general_menu_t about_menu;
  about_menu.options         = options_macros;
  about_menu.banner          = "List Macros";
  about_menu.options_count   = sizeof(options_macros) / sizeof(char*);
  about_menu.selected_option = 0;
  about_menu.select_cb       = onMenuListMacroSelection;
  about_menu.exit_cb         = menuOSMacros;
  general_menu(about_menu);
}

void menuOSMacros(void){
  general_menu_t about_menu;
  about_menu.options         = options_os_macros;
  about_menu.banner          = "OS Macros";
  about_menu.options_count   = sizeof(options_os_macros) / sizeof(char*);
  about_menu.selected_option = 0;
  about_menu.select_cb       = onMenuOsMacroSelection;
  about_menu.exit_cb         = menuApps;
  general_menu(about_menu);
}

void menuMain(void){
  general_menu_t main_menu = {
    .options         = options_main,
    .banner          = "PWNED X8",
    .options_count   = sizeof(options_main) / sizeof(char*),
    .selected_option = 0,
    .select_cb       = onMenuSelection,
    .exit_cb         = NULL,
  };
  general_menu(main_menu);
}

void showSplashScreen(void){
  screenClear();
  screenShowBitmap(&bitmap_pwned, 0, 16);
  screenDisplay();
}