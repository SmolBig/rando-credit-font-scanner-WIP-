#include "Bitmap.h"
#include "ImageCodec.h"
#include <tuple>

Bitmap::Bitmap(const std::string& filename) {
  std::tie(width, height, data) = ImageCodec::loadFromFile(filename);
}

Table32BPP Bitmap::convert() const {
  return Table32BPP(*this);
}

void Bitmap::save(const std::string& filename) {
  ImageCodec::saveToFile(filename, std::make_tuple((UINT)width, (UINT)height, data));
}

Bitmap::Bitmap(const Table32BPP& table) : width(TEXTURE_WIDTH), height(TEXTURE_HEIGHT), data(TEXTURE_PX_LENGTH, GREEN) {
  for(size_t i = 0; i < Table32BPP::TILE_COUNT; i++) {
    auto readHead  = table.data.begin() + Table32BPP::tileOffset(i);
    auto writeHead = data.begin() + tileOffset(i);
    renderTile(readHead, writeHead);
  }
}

void Bitmap::renderTile(std::vector<uint32_t>::const_iterator readHead, std::vector<uint32_t>::iterator writeHead) {
  for(int y = 0; y < Table32BPP::TILE_HEIGHT; y++) {
    std::copy(readHead, readHead + Table32BPP::TILE_WIDTH, writeHead);
    readHead += Table32BPP::TILE_WIDTH;
    writeHead += TEXTURE_WIDTH;
  }
}

size_t Bitmap::tileOffset(size_t i) {
  int row = i / TILES_PER_ROW;
  int col = i % TILES_PER_ROW;

  int x = col * (Table32BPP::TILE_WIDTH + 1);
  int y = (row * Table32BPP::TILE_HEIGHT) + (row / 2);

  return x + (y * TEXTURE_WIDTH);
}

