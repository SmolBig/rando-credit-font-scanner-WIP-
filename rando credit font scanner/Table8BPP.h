#pragma once
#include "Utility.h"

class Table2BPP;
class Bitmap;

class Table8BPP {
public:
  Bitmap generateBitmap() const;
  Table2BPP compact() const;

private:
  ByteArray data;

  friend class Table2BPP;
  Table8BPP(const Table2BPP& data);

  friend class Bitmap;
  Table8BPP(const Bitmap& bmp);

  static size_t tileOffset(size_t i);
  static const uint16_t EXPANDED_REVERSED_BYTES[256];
  static uint16_t interleaveBytes(byte hi, byte lo);

  static constexpr size_t TILE_WIDTH = 8;
  static constexpr size_t TILE_HEIGHT = 8;
  static constexpr size_t TILE_LENGTH = TILE_WIDTH * TILE_HEIGHT;
  static constexpr size_t TILE_COUNT = 512;

};

