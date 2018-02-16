#pragma once
#include "Utility.h"

class Bitmap;
class Table8BPP;

class Table32BPP {
public:
  Bitmap generateBitmap() const;
  Table8BPP compress() const;

private:
  std::vector<uint32_t> data;

  friend class Table8BPP;
  Table32BPP(const Table8BPP& compressed);

  friend class Bitmap;
  Table32BPP(const Bitmap& bmp);

  static uint32_t getColor(byte index);
  static size_t tileOffset(size_t i);

  static constexpr uint32_t PALETTE[4] = {
    0xFF000000, //BLACK
    0xFFFFFFFF, //WHITE
    0xFF0000FF, //BLUE
    0xFFFF0000  //RED
  };

  static constexpr size_t TILE_WIDTH = 8;
  static constexpr size_t TILE_HEIGHT = 8;
  static constexpr size_t TILE_LENGTH = TILE_WIDTH * TILE_HEIGHT;
  static constexpr size_t TILE_COUNT = 512;

};

