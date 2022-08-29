#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>

/*
 * CS    | Chip Select  | A3
 * CD    | Command/Data | A2
 * WR    | Write        | A1
 * RD    | Read         | A0
 * RESET | Reset        | A4
 */

#define c_black   0x0000
#define c_red     0xF800
#define c_yellow  0xFFE0
#define c_green   0x07E0
#define c_blue    0x001F
#define c_gray    0x7BEF
#define c_white   0xFFFF

class LcdDisplay : public Component {
  Adafruit_TFTLCD tft;

  static const int  DEF_ROT       = 3;
  static const int  DEF_POS_X     = 0;
  static const int  DEF_POS_Y     = 0;
  static const int  DEF_BACK_COL  = c_yellow;
  static const int  DEF_TEXT_SIZE = 2;
  static const bool DEF_TEXT_WRAP = false;
  static const int  DEF_TEXT_COL  = c_black;

  void setDefaults() {
    tft.setCursor(DEF_POS_X, DEF_POS_Y);
    tft.setTextColor(DEF_TEXT_COL);
    tft.setTextSize(DEF_TEXT_SIZE);
  }
    
public:
  LcdDisplay(int cs, int cd, int wr, int rd, int res) : tft(cs, cd, wr, rd, res) {
    tft.reset();
    tft.begin(0x9341);
    tft.setRotation(DEF_ROT);
    tft.setTextWrap(DEF_TEXT_WRAP);
    clearScreen();
    setDefaults();
  }

  void clearScreen(int color = DEF_BACK_COL) {
    tft.fillScreen(color);
  }

  void drawTextGeneral(int x, int y, String str, int sz, int color) {
    tft.setCursor(x, y);
    tft.setTextSize(sz);
    tft.setTextColor(color);
    tft.print(str);
    setDefaults();
  }

  void drawTextSize(int x, int y, String str, int sz) {
    drawTextGeneral(x, y, str, sz, DEF_TEXT_COL);
  }

  void drawText(int x, int y, String str) {
    drawTextGeneral(x, y, str, DEF_TEXT_SIZE, DEF_TEXT_COL);
  }
};
