#pragma once
#include "Utility.h"

class SNESTable;
class Bitmap;

class IndexedTable {
public:
  Bitmap generateBitmap() const;
  SNESTable compact() const;

private:
  ByteArray data;

  friend class SNESTable;
  IndexedTable(const SNESTable& data);

  friend class Bitmap;
  IndexedTable(const Bitmap& bmp);

  static size_t tileOffset(size_t i);
  static const uint16_t EXPANDED_REVERSED_BYTES[256];
  static uint16_t interleaveBytes(byte hi, byte lo);

  static constexpr size_t TILE_WIDTH = 8;
  static constexpr size_t TILE_HEIGHT = 8;
  static constexpr size_t TILE_LENGTH = TILE_WIDTH * TILE_HEIGHT;
  static constexpr size_t TILE_COUNT = 512;

};

