#include "Bitmap.h"
#include "ImageCodec.h"
#include <tuple>

Bitmap::Bitmap(const std::string& filename) {
  std::tie(width, height, data) = ImageCodec::loadFromFile(filename);
}

PNGTable Bitmap::convert() const {
  return PNGTable(*this);
}

void Bitmap::save(const std::string& filename) {
  ImageCodec::saveToFile(filename, std::make_tuple(width, height, data));
}

Bitmap::Bitmap(const PNGTable& table) : width(TEXTURE_WIDTH), height(TEXTURE_HEIGHT), data(TEXTURE_PX_LENGTH, GREEN) {
  for(size_t i = 0; i < PNGTable::TILE_COUNT; i++) {
    auto readHead  = table.data.begin() + PNGTable::tileOffset(i);
    auto writeHead = data.begin() + tileOffset(i);
    renderTile(readHead, writeHead);
  }
}

void Bitmap::renderTile(ByteArray::const_iterator readHead, ByteArray::iterator writeHead) {
  for(int y = 0; y < PNGTable::TILE_HEIGHT; y++) {
    std::copy(readHead, readHead + PNGTable::TILE_WIDTH, writeHead);
    readHead += PNGTable::TILE_WIDTH;
    writeHead += TEXTURE_WIDTH;
  }
}

size_t Bitmap::tileOffset(size_t i) {
  int row = i / TILES_PER_ROW;
  int col = i % TILES_PER_ROW;

  int x = col * (PNGTable::TILE_WIDTH + 1);
  int y = (row * PNGTable::TILE_HEIGHT) + (row / 2);

  return x + (y * TEXTURE_WIDTH);
}

