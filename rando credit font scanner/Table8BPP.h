#pragma once
#include "Utility.h"

class Table2BPP;
class Table32BPP;
class BitmapNew;

class Table8BPP {
public:
  Table32BPP expand() const;
  Table2BPP compress() const;

  BitmapNew generateBitmap() const;

private:
  ByteArray data;

  friend class Table2BPP;
  Table8BPP(const Table2BPP& data);

  friend class Table32BPP;
  Table8BPP(const Table32BPP& colorTable);

  friend class BitmapNew;
  Table8BPP(const BitmapNew& bmp);

  static size_t tileOffset(size_t i);
  static uint32_t getColor(byte index);
  static int colorDistance(uColor a, uColor b);
  static byte getNearestIndex(uint32_t color);
  static const uint16_t EXPANDED_REVERSED_BYTES[256];
  static uint16_t interleaveBytes(byte hi, byte lo);

  static constexpr uint32_t PALETTE[5] = {
    0xFF000000, //BLACK
    0xFFFFFFFF, //WHITE
    0xFF0000FF, //BLUE
    0xFFFF0000, //RED
    0xFF00FF00  //GREEN
  };

  static constexpr size_t TILE_WIDTH = 8;
  static constexpr size_t TILE_HEIGHT = 8;
  static constexpr size_t TILE_LENGTH = TILE_WIDTH * TILE_HEIGHT;
  static constexpr size_t TILE_COUNT = 512;

};

