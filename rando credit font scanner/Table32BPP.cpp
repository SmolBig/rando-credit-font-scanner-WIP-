#include "Table32BPP.h"
#include "Bitmap.h"
#include "Table8BPP.h"
#include <algorithm>

Bitmap Table32BPP::generateBitmap() const {
  return Bitmap(*this);
}

Table8BPP Table32BPP::compress() const {
  return Table8BPP(*this);
}

Table32BPP::Table32BPP(const Table8BPP& compressed) : data(compressed.data.size()) {
  auto& cData = compressed.data;
  std::transform(cData.begin(), cData.end(), data.begin(), getColor);
}

uint32_t Table32BPP::getColor(byte index) {
  return PALETTE[index];
}

Table32BPP::Table32BPP(const Bitmap& bmp) {
  for(size_t i = 0; i < TILE_COUNT; i++) {
    auto readHead  = bmp.data.begin() + Bitmap::tileOffset(i);
    auto writeHead = data.begin() + tileOffset(i);
    Bitmap::renderTile(readHead, writeHead);
  }
}

size_t Table32BPP::tileOffset(size_t i) {
  return i * TILE_LENGTH;
}

