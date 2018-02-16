#include "ROM.h"
#include "Table2BPP.h"
#include "Table8BPP.h"
#include "Bitmap.h"

int main() {
  ROM rom("rando.sfc");
  auto two = rom.extractTable();
  auto eight = two.expand();
  auto bmp = eight.generateBitmap();
  bmp.save("font.png");
  return 0;
}

