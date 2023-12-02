#define LGFX_USE_V1
// autodetect ESP32_2432S032
#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

class TouchKey {
  private:
    int _id;
    int _x;
    int _y;
    int _width;
    int _heigh;
    String _txt;
    int _color;
    int _boxed;
    int _size;
    LGFX *_tft;

  public:
    TouchKey(LGFX *tft, int id, int color, int boxed, int size, const String& txt);
    void set(int color, int boxed, int size, const String& txt);
    bool isIn(int x, int y);
    int id();
};