#pragma once
#include "Utility.h"
#include "Table32BPP.h"

class Bitmap {
public:
  Bitmap(const std::string& filename);

  Table32BPP convert() const;
  void save(const std::string& filename);

private:
  size_t width, height;
  std::vector<uint32_t> data;

  friend class Table32BPP;
  Bitmap(const Table32BPP& table);

  static void renderTile(std::vector<uint32_t>::const_iterator readHead, std::vector<uint32_t>::iterator writeHead);
  static size_t tileOffset(size_t i);

  static constexpr uint32_t GREEN = 0xFF00FF00;
  static constexpr size_t TILES_PER_ROW = 16;
  static constexpr size_t TILES_PER_COLUMN = Table32BPP::TILE_COUNT / TILES_PER_ROW;
  static constexpr size_t TEXTURE_WIDTH = TILES_PER_ROW * (Table32BPP::TILE_WIDTH + 1);
  static constexpr size_t TEXTURE_HEIGHT = (TILES_PER_COLUMN * Table32BPP::TILE_HEIGHT) + (TILES_PER_COLUMN / 2);
  static constexpr size_t TEXTURE_PX_LENGTH = TEXTURE_WIDTH * TEXTURE_HEIGHT;

};
