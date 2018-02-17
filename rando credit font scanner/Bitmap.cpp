#include "Bitmap.h"
#include "ImageCodec.h"
#include <tuple>

Bitmap::Bitmap(const std::string& filename) {
  std::tie(width, height, data) = ImageCodec::loadFromFile(filename);
  if((width != TEXTURE_WIDTH) || (height != TEXTURE_HEIGHT)) { throw std::runtime_error("Bitmap::Bitmap(const std::string& filename) - Bitmap dimensions incorrect."); }
}

IndexedTable Bitmap::convert() const {
  return IndexedTable(*this);
}

void Bitmap::save(const std::string& filename) {
  ImageCodec::saveToFile(filename, std::make_tuple(width, height, data));
}

Bitmap::Bitmap(const IndexedTable& table) : width(TEXTURE_WIDTH), height(TEXTURE_HEIGHT), data(TEXTURE_PX_LENGTH, GREEN) {
  for(size_t i = 0; i < IndexedTable::TILE_COUNT; i++) {
    auto readHead  = table.data.begin() + IndexedTable::tileOffset(i);
    auto writeHead = data.begin() + tileOffset(i);
    renderTile(readHead, IndexedTable::TILE_WIDTH, writeHead, TEXTURE_WIDTH);
  }
}

void Bitmap::renderTile(ByteArray::const_iterator readHead, size_t readStride, ByteArray::iterator writeHead, size_t writeStride) {
  for(int y = 0; y < IndexedTable::TILE_HEIGHT; y++) {
    std::copy(readHead, readHead + IndexedTable::TILE_WIDTH, writeHead);
    readHead += readStride;
    writeHead += writeStride;
  }
}

size_t Bitmap::tileOffset(size_t i) {
  int row = i / TILES_PER_ROW;
  int col = i % TILES_PER_ROW;

  int x = col * (IndexedTable::TILE_WIDTH + 1);
  int y = (row * IndexedTable::TILE_HEIGHT) + (row / 2);

  return x + (y * TEXTURE_WIDTH);
}

