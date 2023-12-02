#include "Arduino.h"
#include "A400CDU.h"
#include "const.h"
#include <list>
#include <iostream>
#include <map>

/**
 * @brief Construct a new A400CDU::A400CDU object
 * 
 * @param tft instance pointer to LGFX
 * 
 * Example of call:
 * LGFX tft = LGFX();
 * A400CDU cdu(&tft);
 * 
 */
A400CDU::A400CDU(LGFX *tft)
{
  _tft = tft;
};

/**
 * @brief Initialize display in landscape mode, black background
 * 
 */
void A400CDU::init()
{
  _tft->init();
  // 1=landscape
  // 0=portrait
  _tft->setRotation(1);
  _tft->fillScreen(TFT_BLACK);

  /*uint16_t calData[5] = { 407, 3489, 276, 3485, 7 };
  _tft->setTouch(calData);*/

}

/**
 * @brief Create a new page with title, clear screen and all configured components (buttons, text...)
 * 
 * @param title page title (centered on first line)
 */
void A400CDU::newPage(const String& title)
{
  _tft->fillScreen(TFT_BLACK);
  m_TouchKeyList.clear();
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  setTitle(title);

}

/**
 * @brief Set centered page title on line 1, without clearing screen
 * 
 * @param title page title (centered on first line)
 */
void A400CDU::setTitle(const String& title)
{
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setTextDatum(TC_DATUM);
  _tft->drawString(title,160,0);
}

/**
 * @brief draw text (not boxed)
 * 
 * @param id position
 * @param color color
 * @param size text size, can be 1,2 or 3
 * @param txt text shown
 */
void A400CDU::addTextHelper(int id, int color, int size, const String& txt)
{
  addTouchKey(id, color, 0, size, txt);
}

/**
 * @brief draw a button
 * 
 * @param id position
 * @param color color
 * @param size text size, can be 1,2 or 3
 * @param txt text shown
 */
void A400CDU::addButtonHelper(int id, int color, int size, const String& txt)
{
  addTouchKey(id, color, 1, size, txt);
}

/**
 * @brief draw a button with dual led status
 * 
 * @param id position
 * @param color color
 * @param state true or false, blue ou grey dual led
 * @param txt text shown
 */
void A400CDU::addButtonLedHelper(int id, int color, bool state, const String& txt)
{
  if (state) {
    addTouchKey(id, color, 1, 5, txt);
  } else {
    addTouchKey(id, color, 1, 4, txt);
  }
}

/**
 * @brief draw a text with dual led status
 * 
 * @param id position
 * @param color color
 * @param state true or false, display blue or grey leds
 * @param txt text shown
 */
void A400CDU::addStatusLedHelper(int id, int color, bool state, const String& txt)
{
  if (state) {
    addTouchKey(id, color, 0, 5, txt);
  } else {
    addTouchKey(id, color, 0, 4, txt);
  }
}

/**
 * @brief Add a touch key to keys iterator
 * 
 * @param id position according to key matrix
 * @param color see color codes in const.h
 * @param boxed 0 for no box, 1 for box
 * @param size text size, used to define type and size, 0 = slider, 1 or 2 or 3 = textsize, 4 button with grey light (inactive), 5 button with blue light (active)
 * @param txt text shown
 */
void A400CDU::addTouchKey(int id, int color, int boxed,int size, const String& txt)
{
  // check is already exists then change it
  std::list<TouchKey*>::const_iterator end = m_TouchKeyList.end();
  for (std::list<TouchKey*>::const_iterator it = m_TouchKeyList.begin(); it != end; ++it)
  {
    if ((*it)->id()==id)
    {
      (*it)->set(color, boxed, size, txt);
      return;
    }
  }
  // else create it
  m_TouchKeyList.push_front(new TouchKey(_tft, id, color, boxed, size, txt));
}

/**
 * @brief Set LED on or off
 * 
 * @param txt les status: "0" for off, "1" for on 
 */
void A400CDU::writeLED(const String& txt)
{
  if (txt=="1") {
    digitalWrite(17,1);
  } else {
    digitalWrite(17,0);
  }
  return;
}

/**
 * @brief find key matrix position depending on x and y
 * 
 * @param x coodinate of pressed pixel
 * @param y coodinate of pressed pixel
 * @return int position in key matrix (see const.h)
 */
int A400CDU::TouchKeyPressed(int x, int y)
{
  std::list<TouchKey*>::const_iterator end = m_TouchKeyList.end();
  for (std::list<TouchKey*>::const_iterator it = m_TouchKeyList.begin(); it != end; ++it)
  {
    if ((*it)->isIn(x, y))
    {
      //delay(100);
      return (*it)->id();
    }
  }
  return -1;
}

/**
 * @brief Perform touchscreen polling
 * 
 * @return int pressed key (position given by key matrix in const.h)
 */
int A400CDU::poll()
{
  uint16_t x = 0, y = 0;
  if (_tft->getTouch(&x,&y))
  {
    int tp=TouchKeyPressed(x,y);
    if (tp!=-1) {return tp;}
  }
  return -1;
}