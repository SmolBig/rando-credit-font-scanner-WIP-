#include "BitmapNew.h"
#include "ImageCodec.h"
#include <tuple>

BitmapNew::BitmapNew(const std::string& filename) {
  //std::tie(width, height, data) = ImageCodec::loadFromFile(filename);
}

Table8BPP BitmapNew::convert() const {
  return Table8BPP(*this);
}

void BitmapNew::save(const std::string& filename) {
  ImageCodec::savePackedToFile(filename, data);
}

BitmapNew::BitmapNew(const Table8BPP& table) : width(TEXTURE_WIDTH), height(TEXTURE_HEIGHT), data(TEXTURE_PX_LENGTH, GREEN) {
  for(size_t i = 0; i < Table8BPP::TILE_COUNT; i++) {
    auto readHead  = table.data.begin() + Table8BPP::tileOffset(i);
    auto writeHead = data.begin() + tileOffset(i);
    renderTile(readHead, writeHead);
  }
}

void BitmapNew::renderTile(ByteArray::const_iterator readHead, ByteArray::iterator writeHead) {
  for(int y = 0; y < Table8BPP::TILE_HEIGHT; y++) {
    std::copy(readHead, readHead + Table8BPP::TILE_WIDTH, writeHead);
    readHead += Table8BPP::TILE_WIDTH;
    writeHead += TEXTURE_WIDTH;
  }
}

size_t BitmapNew::tileOffset(size_t i) {
  int row = i / TILES_PER_ROW;
  int col = i % TILES_PER_ROW;

  int x = col * (Table8BPP::TILE_WIDTH + 1);
  int y = (row * Table8BPP::TILE_HEIGHT) + (row / 2);

  return x + (y * TEXTURE_WIDTH);
}

