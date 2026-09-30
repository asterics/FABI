#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <sstream>
#include <map>

// Mock definitions for Arduino / hardware
#define PROGMEM
#define pgm_read_byte_near(x) (*(uint8_t*)(x))
#define strcmp_FM strcmp
#define uint_farptr_t_FM const char *
#define FLIPMOUSE 1

#include "../FabiWare/src/parser.h"
#include "../FabiWare/src/commands.h"

// Mock Serial
struct MockSerial {
    std::vector<std::string> output_lines;
    std::string current_line;

    void print(const char* s) { current_line += s; }
    void print(int n) { current_line += std::to_string(n); }
    void print(const std::string& s) { current_line += s; }
    void println(const char* s) { current_line += s; output_lines.push_back(current_line); current_line.clear(); }
    void println(const std::string& s) { current_line += s; output_lines.push_back(current_line); current_line.clear(); }
    void println(int n) { current_line += std::to_string(n); output_lines.push_back(current_line); current_line.clear(); }
    void clear() { output_lines.clear(); current_line.clear(); }
    std::string last() { return output_lines.empty() ? "" : output_lines.back(); }
} Serial;

// Mock global slot settings
#define MAX_NAME_LEN 15
#define MAX_SLOTS_IN_EEPROM 10

struct MockSlotSettings {
    char slotName[MAX_NAME_LEN];
} slotSettings;

uint8_t actSlot = 0;
bool displayUpdated = false;
void displayUpdate() { displayUpdated = true; }
void release_all() {}

// Mock slot storage
std::map<int8_t, std::string> mockSlots;

int8_t slotnameToNumber(char const * slotname) {
    for (auto const& pair : mockSlots) {
        if (pair.second == slotname) {
            return pair.first;
        }
    }
    return -1;
}

void saveToEEPROMSlotNumber(int8_t nr, char const * slotname) {
    mockSlots[nr] = slotSettings.slotName;
}

const char ERRORMESSAGE_NOT_FOUND[] = "E: not found";
const char ERRORMESSAGE_EEPROM_FULL[] = "E: eeprom full";
const char ERRORMESSAGE_NAME_EXISTS[] = "E: name exists";

int8_t renameSlot(char const * newName)
{
  if (!newName) return 0;

  uint8_t len = strlen(newName);
  if ((len == 0) || (len >= MAX_NAME_LEN)) return 0;

  if ((actSlot < 0) || (actSlot >= MAX_SLOTS_IN_EEPROM)) return 0;

  int8_t existingSlot = slotnameToNumber(newName);
  if ((existingSlot != -1) && (existingSlot != actSlot)) {
    return -1;
  }

  strncpy(slotSettings.slotName, newName, MAX_NAME_LEN - 1);
  slotSettings.slotName[MAX_NAME_LEN - 1] = '\0';

  saveToEEPROMSlotNumber(actSlot, slotSettings.slotName);

  return 1;
}

const struct atCommandType atCommands[] PROGMEM = {
  /***** ID and ButtonMode *****/
  {"ID"  , PARTYPE_NONE }, {"BM"  , PARTYPE_UINT }, 
  /***** mouse *****/
  {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE }, {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, 
  {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE }, {"PM"  , PARTYPE_NONE },  
  {"HL"  , PARTYPE_NONE }, {"HR"  , PARTYPE_NONE }, {"HM"  , PARTYPE_NONE },
  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
  {"TL"  , PARTYPE_NONE }, {"TR"  , PARTYPE_NONE }, {"TM"  , PARTYPE_NONE },
  {"WU"  , PARTYPE_NONE }, {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, 
  {"MX"  , PARTYPE_INT  }, {"MY"  , PARTYPE_INT  }, 
  /***** joystick *****/
  {"J0"  , PARTYPE_INT  }, {"J1"  , PARTYPE_INT  }, {"J2"  , PARTYPE_INT  }, {"J3"  , PARTYPE_INT  },  
  {"J4"  , PARTYPE_INT  }, {"J5"  , PARTYPE_INT  }, 
  {"JP"  , PARTYPE_INT  }, {"JR"  , PARTYPE_INT  }, {"JH"  , PARTYPE_INT  },
  /***** keyboard *****/
  {"KW"  , PARTYPE_STRING}, {"KP"  , PARTYPE_STRING}, {"KH"  , PARTYPE_STRING}, {"KT"  , PARTYPE_STRING },
  {"KR"  , PARTYPE_STRING}, {"RA"  , PARTYPE_NONE },  {"KL"  , PARTYPE_STRING },
  /***** slots *****/
  {"SA"  , PARTYPE_STRING}, {"LO"  , PARTYPE_STRING}, {"LA"  , PARTYPE_NONE }, {"LI"  , PARTYPE_NONE },
  {"NE"  , PARTYPE_NONE },  {"DE"  , PARTYPE_STRING }, {"RS"  , PARTYPE_NONE }, {"RE"  , PARTYPE_NONE },
  {"SN"  , PARTYPE_STRING },
  /***** slot settings & reporting *****/
  {"NC"  , PARTYPE_NONE }, {"BT"  , PARTYPE_UINT }, {"SC"  , PARTYPE_STRING },
  {"SR"  , PARTYPE_NONE }, {"ER"  , PARTYPE_NONE }, {"CA"  , PARTYPE_NONE },
  /***** macros *****/
  {"MA"  , PARTYPE_STRING}, {"WA"  , PARTYPE_UINT  }, 
  /***** sip&puff *****/
  {"TS"  , PARTYPE_UINT },  {"TP"  , PARTYPE_UINT }, {"SP"  , PARTYPE_UINT }, {"SS"  , PARTYPE_UINT },
  /***** infrared *****/
  {"IR"  , PARTYPE_STRING}, {"IP"  , PARTYPE_STRING}, {"IH"  , PARTYPE_STRING }, {"IS"  , PARTYPE_NONE },
  {"IC"  , PARTYPE_STRING}, {"IW"  , PARTYPE_NONE }, {"IL"  , PARTYPE_NONE }, {"IT"  , PARTYPE_UINT  },
  /***** 2D mouse cursor control *****/
  {"MM"  , PARTYPE_UINT }, {"SW"  , PARTYPE_NONE },
  {"AX"  , PARTYPE_UINT }, {"AY"  , PARTYPE_UINT }, {"DX"  , PARTYPE_UINT }, {"DY"  , PARTYPE_UINT },
  {"MS"  , PARTYPE_UINT }, {"AC"  , PARTYPE_UINT }, {"RO"  , PARTYPE_UINT }, {"SB"  , PARTYPE_UINT },
  /***** audio feedback *****/
  {"AT"  , PARTYPE_STRING}, {"AP"  , PARTYPE_STRING}, {"AR" , PARTYPE_STRING}, {"AL"  , PARTYPE_NONE},
  {"AV"  , PARTYPE_UINT },  {"AB"  , PARTYPE_UINT },
  /***** timing *****/
  {"AD"  , PARTYPE_UINT }, {"LP"  , PARTYPE_UINT }, {"MP"  , PARTYPE_UINT },
  #ifdef FLIPMOUSE
    /***** BT-Housekeeping / FM-Only *****/
    {"BC"  , PARTYPE_STRING}, {"BR"  , PARTYPE_UINT }, {"UG", PARTYPE_NONE },
  #endif
};

void performCommand(uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement)
{
    switch(cmd) {
    case CMD_SN:
      release_all();
      if (keystring) {
        while (*keystring == ' ' || *keystring == '\t') keystring++;
        int len = strlen(keystring);
        while (len > 0 && (keystring[len-1] == ' ' || keystring[len-1] == '\r' || keystring[len-1] == '\n' || keystring[len-1] == '\t')) {
          keystring[--len] = '\0';
        }
      }
      if (keystring && (strlen(keystring) > 0)) {
        if (strlen(keystring) < MAX_NAME_LEN) {
          int8_t res = renameSlot(keystring);
          if (res == 1) {
            displayUpdate();
            Serial.println("OK");
          } else if (res == -1) {
            Serial.println(ERRORMESSAGE_NAME_EXISTS);
          } else {
            Serial.println("?");
          }
        } else {
          Serial.println("?");
        }
      } else {
        Serial.println(slotSettings.slotName);
      }
      break;
    default:
      break;
    }
}

void strup (char * str)
{
  if (!str) return;
  while (*str)
  {
    if ((*str >= 'a') && (*str <= 'z')) *str = *str - 'a' + 'A';
    str++;
  }
}

void test_parseCommand(char * cmdstr)
{
  int8_t cmd = -1;
  int16_t num = 0;
  char * lastpos = cmdstr + strlen(cmdstr);

  char * actpos = strtok(cmdstr, " ");

  if (actpos)
  {
    int i;
    strup(actpos);

    for (i = 0; (i < NUM_COMMANDS) && (cmd == -1); i++)
    {
      if (!strcmp_FM(actpos, (uint_farptr_t_FM)atCommands[i].atCmd))  {
        switch (pgm_read_byte_near(&(atCommands[i].partype)))
        {
          case PARTYPE_UINT: actpos = strtok(NULL, " "); break;
          case PARTYPE_INT:  actpos = strtok(NULL, " "); break;
          case PARTYPE_STRING: actpos += 3; if (actpos>lastpos) actpos=lastpos; cmd = i; break;
          default: cmd = i; actpos = 0; break;
        }
      }
    }
  }

  if (cmd > -1) {
    performCommand(cmd, num, actpos, 0);
  } else {
    Serial.println("?");
  }
}

void runCommand(const std::string& input) {
    char buf[128];
    strncpy(buf, input.c_str(), sizeof(buf));
    buf[sizeof(buf)-1] = '\0';
    test_parseCommand(buf);
}

int main() {
    std::cout << "Starting C++ AT SN unit tests..." << std::endl;

    // Setup initial slots
    actSlot = 0;
    strncpy(slotSettings.slotName, "default", MAX_NAME_LEN);
    mockSlots[0] = "default";
    mockSlots[1] = "slot2";

    // Test 1: Query current slot name: "SN"
    Serial.clear();
    runCommand("SN");
    assert(Serial.last() == "default");
    std::cout << "Test 1 Passed: Query slot name -> " << Serial.last() << std::endl;

    // Test 2: Rename current slot: "SN mouse"
    Serial.clear();
    displayUpdated = false;
    runCommand("SN mouse");
    assert(Serial.last() == "OK");
    assert(std::string(slotSettings.slotName) == "mouse");
    assert(mockSlots[0] == "mouse");
    assert(displayUpdated == true);
    std::cout << "Test 2 Passed: Rename slot -> OK, new name: " << slotSettings.slotName << std::endl;

    // Test 3: Query renamed slot
    Serial.clear();
    runCommand("SN");
    assert(Serial.last() == "mouse");
    std::cout << "Test 3 Passed: Query updated name -> " << Serial.last() << std::endl;

    // Test 4: Rename with leading/trailing spaces: "SN   keys  "
    Serial.clear();
    runCommand("SN   keys  ");
    assert(Serial.last() == "OK");
    assert(std::string(slotSettings.slotName) == "keys");
    assert(mockSlots[0] == "keys");
    std::cout << "Test 4 Passed: Whitespace trimmed -> OK, new name: " << slotSettings.slotName << std::endl;

    // Test 5: Conflict check - rename to existing slot name in slot 1: "SN slot2"
    Serial.clear();
    runCommand("SN slot2");
    assert(Serial.last() == "E: name exists");
    assert(std::string(slotSettings.slotName) == "keys"); // Name shouldn't change
    std::cout << "Test 5 Passed: Conflict detected -> " << Serial.last() << std::endl;

    // Test 6: Rename to the same name already on current slot should succeed
    Serial.clear();
    runCommand("SN keys");
    assert(Serial.last() == "OK");
    assert(std::string(slotSettings.slotName) == "keys");
    std::cout << "Test 6 Passed: Rename to self -> " << Serial.last() << std::endl;

    // Test 7: Name exceeding MAX_NAME_LEN (15 chars)
    Serial.clear();
    runCommand("SN verylongslotnamethatexceeds");
    assert(Serial.last() == "?");
    assert(std::string(slotSettings.slotName) == "keys");
    std::cout << "Test 7 Passed: Name too long rejected -> " << Serial.last() << std::endl;

    // Test 8: Empty name with space: "SN "
    Serial.clear();
    runCommand("SN ");
    assert(Serial.last() == "keys"); // Treated as query
    std::cout << "Test 8 Passed: Empty name parameter treated as query -> " << Serial.last() << std::endl;

    std::cout << "ALL C++ AT SN unit tests PASSED successfully!" << std::endl;
    return 0;
}
