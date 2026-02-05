#include <cmd.h>
#include <flags.h>
#include <EEPROM.h>
#include <shared.h>

SerialCommand sCmd;

static void saveName(const char *name) {
  uint16_t signature = SIGNATURE_VALUE;
  EEPROM.put(EEPROM_SIGNATURE_ADDR, signature);
  for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (name[i] == '\0') {
      EEPROM.write(EEPROM_NAME_ADDR + i, '\0');
      break;
    }
    EEPROM.write(EEPROM_NAME_ADDR + i, name[i]);
  }
  EEPROM.write(EEPROM_NAME_ADDR + MAX_NAME_LEN, '\0');
  EEPROM.commit();
}

static void cmdDefaultHandler(const char *command){
  Serial.println("Unknown command");
}

static void cmdHandleFlag(){
  char *arg = sCmd.next();
  if (arg != NULL){
    if(strcmp(arg, FLAG_SERIAL_FUZZ) == 0){
      Serial.println("Correct!");
    }else{
      Serial.println("Incorrect!");
    }
  }else{
    Serial.println("No argument");
  }
}

static void cmdHandleUsername(){
  char *arg = sCmd.next();
  if (arg != NULL){
    Serial.print("Hello: ");
    Serial.println(arg);
    saveName(arg);
  }else{
    Serial.println("No argument");
  }
}

void cmdBegin(void){
  sCmd.setDefaultHandler(cmdDefaultHandler);
  sCmd.addCommand("set_flag", cmdHandleFlag);
  sCmd.addCommand("username", cmdHandleUsername);
}

void cmdRead(void){
  sCmd.readSerial();
}