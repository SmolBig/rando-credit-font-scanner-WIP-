#pragma once
#include "Utility.h"
#include "Table8BPP.h"

class BitmapNew {
public:
  BitmapNew(const std::string& filename);

  Table8BPP convert() const;
  void save(const std::string& filename);

private:
  size_t width, height;
  ByteArray data;

  friend class Table8BPP;
  BitmapNew(const Table8BPP& table);

  static void renderTile(ByteArray::const_iterator readHead, ByteArray::iterator writeHead);
  static size_t tileOffset(size_t i);

  static constexpr byte GREEN = 4;
  static constexpr size_t TILES_PER_ROW = 16;
  static constexpr size_t TILES_PER_COLUMN = Table8BPP::TILE_COUNT / TILES_PER_ROW;
  static constexpr size_t TEXTURE_WIDTH = TILES_PER_ROW * (Table8BPP::TILE_WIDTH + 1);
  static constexpr size_t TEXTURE_HEIGHT = (TILES_PER_COLUMN * Table8BPP::TILE_HEIGHT) + (TILES_PER_COLUMN / 2);
  static constexpr size_t TEXTURE_PX_LENGTH = TEXTURE_WIDTH * TEXTURE_HEIGHT;

};
