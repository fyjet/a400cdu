#include "Arduino.h"
#include "TouchKey.h"
#include "const.h"


  /**
   * @brief Constructor
   * 
   * @param id SideKey matrix coordinates XX, see const.h
   * @param color encoded color, see const.h
   * @param boxed 0 = non boxed, 1 = box
   * @param size  0 = slider, 1 or 2 or 3 = textsize, 4 button with grey light (inactive), 5 button with blue light (active)
   * @param txt text to display
   */
  TouchKey::TouchKey(LGFX *tft, int id, int color, int boxed, int size, const String& txt)
  {
    _tft = tft;
    _tft->setTextDatum(TL_DATUM);
    _id=id;
    _txt=txt;
    _width=_txt.length()*18+2;
    _boxed=boxed;
    _size=size;

    set(color, _boxed, _size, txt);
  };

  void TouchKey::set(int color, int boxed, int size, const String& txt)
  {
    _txt=txt;
    _boxed=boxed;
    _size=size;

    if (color==WHITE) {_color=TFT_WHITE;}
    if (color==YELLOW) {_color=TFT_YELLOW;}
    if (color==GREEN) {_color=TFT_GREEN;}
    if (color==BLACK) {_color=TFT_BLACK;}
    if (color==BLUE) {_color=TFT_BLUE;}
    if (color==GREY) {_color=TFT_GREY;}
    if (color==RED) {_color=TFT_RED;}

    /* draw text */
    if (_size>0 and _size<4) {
      _tft->setTextSize(_size);
      _width=_txt.length()*6*_size+2;
      _heigh=_size*8+2;
      _tft->setTextColor(_color,TFT_BLACK);
      _y=(_id%10)*30-28;

      // Left align
      if (_id<10) {
        _x=0;
      }
      if (_id>10 and _id<20) {
        _x=80;
      }
      // Right align
      if (_id>20 and _id<30) {
        _x=240-_width;
      }
      if (_id>30) {
        _x=320-_width;
      }
      _tft->drawString(_txt, _x+2, _y+2);
      if (_boxed==1) {
        _tft->drawRect(_x, _y, _width, _heigh, TFT_GREEN);
      } else {
        _tft->drawRect(_x, _y, _width, _heigh, TFT_BLACK);
      }
    }
    /* button with light indicator*/
    if (_size>=4) {
      _tft->setTextSize(3);
      _width=76;
      _heigh=26;
      _tft->setTextColor(_color,TFT_BLACK);
      _y=(_id%10)*30-28;

      // Left align
      if (_id<10) {
        _x=0;
      }
      if (_id>10 and _id<20) {
        _x=80;
      }
      // Right align
      if (_id>20 and _id<30) {
        _x=240-_width;
      }
      if (_id>30) {
        _x=320-_width;
      }
      _tft->drawString(_txt, _x+4, _y+2);
      if (_boxed==1) {
        _tft->drawRoundRect(_x, _y, 76, _heigh, 4, TFT_GREEN);
      } else {
        _tft->drawRoundRect(_x, _y, 76, _heigh, 4, TFT_BLACK);
      }

      if (_size==5) {
        //_tft->fillRect(_x+52,_y+2,22,22,TFT_BLUE);
        _tft->fillCircle(_x+67, _y+7, 5, TFT_BLUE);
        _tft->fillCircle(_x+67, _y+19, 5, TFT_BLUE);
      } else {
        //_tft->fillRect(_x+52,_y+2,22,22,TFT_GREY);
        _tft->fillCircle(_x+67, _y+7, 5, TFT_GREY);
        _tft->fillCircle(_x+67, _y+19, 5, TFT_GREY);
      }
    }
    /* draw slider */
    if (_size==0) {
      _width=160;
      _heigh=26;
      _y=(_id%10)*30-28;

      if (_id<10) {
        _x=0;

      } else {
        _x=160;
      }

      if (boxed==1) {
        _tft->drawRect(_x, _y, 160, _heigh, TFT_GREEN);
      } else {
        _tft->drawRect(_x, _y, 160, _heigh, TFT_BLACK);
      }
      _tft->fillRect(_x+2,_y+2,156,22,_color);
      _tft->fillRect(_x+4,_y+4,152,18,TFT_BLACK);
      int _value;
      sscanf(txt.c_str(), "%d", &_value);
      _tft->fillTriangle(_x+79+_value,_y+18,_x+71+_value,_y+6, _x+87+_value, _y+6, _color);
    }


  }

  bool TouchKey::isIn(int x, int y)
  {
    if ((x>_x && x<(_x+_width)) && (y>_y && y<(_y+_heigh))) {
      // video inverted when clicked
      if (_boxed==1) {
        if (_size>0 and _size<4) {
          _tft->setTextColor(TFT_BLACK,TFT_GREEN);
          _tft->setTextSize(_size);
          _tft->drawString(_txt, _x+2, _y+2);
          delay(80);
          _tft->setTextColor(_color,TFT_BLACK);
          _tft->drawString(_txt, _x+2, _y+2);
          _tft->drawRect(_x, _y, _width, _size*8+2, TFT_GREEN);
         return true;
       }
       if (_size==0) {
         _tft->fillRect(_x+2,_y+2,156,22,TFT_GREEN);
         delay(80);
         _tft->fillRect(_x+2,_y+2,156,22,_color);
         _tft->fillRect(_x+4,_y+4,152,18,TFT_BLACK);
         int _value;
         sscanf(_txt.c_str(), "%d", &_value);
         _tft->fillTriangle(_x+79+_value,_y+18,_x+71+_value,_y+6, _x+87+_value, _y+6, _color);
         return true;
       }
       if (_size==4 or _size==5) {
         _tft->setTextColor(TFT_BLACK,TFT_GREEN);
         _tft->setTextSize(3);
         _tft->drawString(_txt, _x+4, _y+2);
         delay(80);
         _tft->setTextColor(_color,TFT_BLACK);
         _tft->drawString(_txt, _x+4, _y+2);
         _tft->drawRoundRect(_x, _y, 76, 26, 4, TFT_GREEN);
         return true;
       }
      }
    }
    return false;
  }

  int TouchKey::id()
  {
    return _id;
  }
