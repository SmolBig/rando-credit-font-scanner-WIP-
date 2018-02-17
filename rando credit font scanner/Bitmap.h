#pragma once
#include "Utility.h"
#include "IndexedTable.h"

class Bitmap {
public:
  Bitmap(const std::string& filename);

  IndexedTable convert() const;
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

  friend class IndexedTable;
  Bitmap(const IndexedTable& table);

  static void renderTile(ByteArray::const_iterator readHead, size_t readStride, ByteArray::iterator writeHead, size_t writeStride);
  static size_t tileOffset(size_t i);

  static constexpr byte GREEN = 4;
  static constexpr size_t TILES_PER_ROW = 16;
  static constexpr size_t TILES_PER_COLUMN = IndexedTable::TILE_COUNT / TILES_PER_ROW;
  static constexpr size_t TEXTURE_WIDTH = TILES_PER_ROW * (IndexedTable::TILE_WIDTH + 1);
  static constexpr size_t TEXTURE_HEIGHT = (TILES_PER_COLUMN * IndexedTable::TILE_HEIGHT) + (TILES_PER_COLUMN / 2);
  static constexpr size_t TEXTURE_PX_LENGTH = TEXTURE_WIDTH * TEXTURE_HEIGHT;

};
