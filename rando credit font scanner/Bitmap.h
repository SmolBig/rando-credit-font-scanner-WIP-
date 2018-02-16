#pragma once
#include "Utility.h"
#include "Table8BPP.h"

class Bitmap {
public:
  Bitmap(const std::string& filename);

  Table8BPP convert() const;
  void save(const std::string& filename);

  static constexpr uint32_t PALETTE[5] = {
    0xFF000000, //BLACK
    0xFFFFFFFF, //WHITE
    0xFF0000FF, //BLUE
    0xFFFF0000, //RED
    0xFF00FF00  //GREEN
  };

private:
  size_t width, height;
  ByteArray data;

  friend class Table8BPP;
  Bitmap(const Table8BPP& table);

  static void renderTile(ByteArray::const_iterator readHead, ByteArray::iterator writeHead);
  static size_t tileOffset(size_t i);

  static constexpr byte GREEN = 4;
  static constexpr size_t TILES_PER_ROW = 16;
  static constexpr size_t TILES_PER_COLUMN = Table8BPP::TILE_COUNT / TILES_PER_ROW;
  static constexpr size_t TEXTURE_WIDTH = TILES_PER_ROW * (Table8BPP::TILE_WIDTH + 1);
  static constexpr size_t TEXTURE_HEIGHT = (TILES_PER_COLUMN * Table8BPP::TILE_HEIGHT) + (TILES_PER_COLUMN / 2);
  static constexpr size_t TEXTURE_PX_LENGTH = TEXTURE_WIDTH * TEXTURE_HEIGHT;

};
