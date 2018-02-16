#include "ROM.h"
#include "Table2BPP.h"
#include "Table8BPP.h"
#include "Table32BPP.h"
#include "Bitmap.h"
#include "BitmapNew.h"

int main() {
  ROM rom("rando.sfc");
  auto two = rom.extractTable();
  auto eight = two.expand();

  auto png = eight.generateBitmap();
  png.save("test.png");
  return 0;

  auto color = eight.expand();
  auto bmp = color.generateBitmap();
  bmp.save("font.png");
}

