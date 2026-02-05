#ifndef __MACROS_H
#define __MACROS_H
#include "Keyboard.h"

#define MACOS_IFCONFIG "ifconfig | grep inet\n"
#define LINUX_IFCONFIG "ip a | grep inet\n"
#define WINDOWS_IFCONFIG "ipconfig\n"

#define GLOBAL_RICK_URL "https://youtu.be/dQw4w9WgXcQ\?si=GST_F_0Pi_daemgN"

void macrosBegin(void);
void macroRunTerminalWindows(const char* cmd);
void macroRunTerminalLinux(const char* cmd);
void macroRunTerminalMacos(const char* cmd);
#endif