#include "ROM.h"
#include "SNESTable.h"
#include "PNGTable.h"
#include "Bitmap.h"

int main() {
  Bitmap bitmap("font.png");
  auto derp = bitmap.convert();
  auto herp = derp.compact();
  ROM hurr("rando.sfc");
  hurr.injectTable(herp);
  hurr.save("test.sfc");

  return 0;

  ROM rom("rando.sfc");
  auto two = rom.extractTable();
  auto eight = two.expand();
  auto bmp = eight.generateBitmap();
  bmp.save("font.png");
}

