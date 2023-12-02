#ifndef _DISPLAY_h
#define _DISPLAY_h
#include <list>
#include <iostream>

#define LGFX_USE_V1
// autodetect ESP32_2432S032
#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

#include "const.h"
#include <map>
#include "TouchKey.h"

class A400CDU {

  private:
  std::list<TouchKey*> m_TouchKeyList;
  LGFX* _tft;
  int _mode;
  int _oldmode;
  
  void addTouchKey(int id, int color, int boxed, int size, const String& txt);
  int TouchKeyPressed(int x, int y);

  public:
  A400CDU(LGFX *tft);
  
  void init();
  
  void newPage(const String& title);
  void setTitle(const String& title);
    
  void addTextHelper(int id, int color, int size, const String& txt);
  void addButtonHelper(int id, int color, int size, const String& txt);
  void addButtonLedHelper(int id, int color, bool state, const String& txt);
  void addStatusLedHelper(int id, int color, bool state, const String& txt);
  void writeLED(const String& txt);

  int poll();

};

#endif
