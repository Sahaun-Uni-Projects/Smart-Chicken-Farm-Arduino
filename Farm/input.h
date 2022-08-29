#include<Keypad.h>

class Keypad4x4 : public Component {
  static const int BUFF_MAX_SIZE = 1024;
  
  static const byte KP_ROWS = 4;
  static const byte KP_COLS = 4;
  byte KP_ROW_PINS[KP_ROWS] = {46, 47, 48, 49};
  byte KP_COL_PINS[KP_COLS] = {50, 51, 52, 53};
  const char KP_MAP[KP_ROWS][KP_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
  };
  Keypad kpad = create();
  String buff = "";
  char key_last = NO_KEY;
  char key_last_valid = NO_KEY;
  int buff_max_size = BUFF_MAX_SIZE;

  Keypad create() {
    return Keypad(makeKeymap(KP_MAP), KP_ROW_PINS, KP_COL_PINS, KP_ROWS, KP_COLS);
  }

  void bufferAppend(char ch) {
    if (buff.length() < buff_max_size) {
      buff += ch;
    }
  }

  bool isDigit(char ch) {
    return ((ch >= '0') && (ch <= '9'));
  }
  
public:
  Keypad4x4(byte row_pins[], byte col_pins[], int buff_max_size = BUFF_MAX_SIZE) : buff_max_size(buff_max_size) {
    for (int i = 0; i < 4; ++i) {
      KP_ROW_PINS[i] = row_pins[i];
      KP_COL_PINS[i] = col_pins[i];
    }
    kpad = create();
  }

  virtual void stateOn() {
    char ch = kpad.getKey();
    key_last = ch;
    if (ch != NO_KEY) {
      key_last_valid = ch;
      if (isDigit(ch)) bufferAppend(ch);
    }
  }

  bool keyPressed() {
    return (key_last != NO_KEY);
  }
  
  char getKey() {
    return key_last;
  }
  
  char getKeyValid() {
    return key_last_valid;
  }

  void bufferClear() {
    buff = "";
  }

  char bufferPop() {
    int len = buff.length();
    if (len == 0) return NO_KEY;
    char ch = buff[len-1];
    buff.remove(len-1, 1);
    return ch;
  }

  String bufferGet() {
    return buff;
  }
};
