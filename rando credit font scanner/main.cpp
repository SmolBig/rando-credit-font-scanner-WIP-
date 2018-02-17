#include "ROM.h"
#include "SNESTable.h"
#include "IndexedTable.h"
#include "Bitmap.h"

#define EXTRACT

int main() {
#ifdef EXTRACT
  ROM rom("rando.sfc");
  SNESTable snes = rom.extractTable();
  IndexedTable indices = snes.expand();
  Bitmap bmp = indices.generateBitmap();
  bmp.save("font.png");
#else
  Bitmap bmp("edit.png");
  IndexedTable indices = bmp.convert();
  SNESTable snes = indices.compact();
  ROM rom("rando.sfc");
  rom.injectTable(snes);
  rom.save("mod.sfc");
#endif
}

