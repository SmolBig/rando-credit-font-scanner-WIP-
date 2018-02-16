#include "Table8BPP.h"
#include "Table2BPP.h"
#include <algorithm>
#include "Bitmap.h"

Bitmap Table8BPP::generateBitmap() const {
  return Bitmap(*this);
}

Table2BPP Table8BPP::compact() const {
  return Table2BPP(*this);
}

Table8BPP::Table8BPP(const Table2BPP& compressed) {
  auto& cData = compressed.data;

  data.reserve(cData.size() * Table2BPP::TEXELS_PER_BYTE);

  for(size_t i = 0; i < (cData.size() - 1); i += 2) {
    uint16_t interleaved = interleaveBytes(cData[i], cData[i+1]);
    for(int s = 0; s < Table2BPP::TEXELS_PER_BYTE * 2; s++) {
      data.push_back(interleaved & 0b11);
      interleaved >>= 2;
    }
  }
}

Table8BPP::Table8BPP(const Bitmap& bmp) {
  throw std::exception("Table8BPP::Table8BPP(const Bitmap& bmp) - Not yet implemented."); //~~_
}

size_t Table8BPP::tileOffset(size_t i) {
  return i * TILE_LENGTH;
}

const uint16_t Table8BPP::EXPANDED_REVERSED_BYTES[256] = {
  0b0000000000000000, 0b1100000000000000, 0b0011000000000000, 0b1111000000000000, 0b0000110000000000, 0b1100110000000000, 0b0011110000000000, 0b1111110000000000,
  0b0000001100000000, 0b1100001100000000, 0b0011001100000000, 0b1111001100000000, 0b0000111100000000, 0b1100111100000000, 0b0011111100000000, 0b1111111100000000,
  0b0000000011000000, 0b1100000011000000, 0b0011000011000000, 0b1111000011000000, 0b0000110011000000, 0b1100110011000000, 0b0011110011000000, 0b1111110011000000,
  0b0000001111000000, 0b1100001111000000, 0b0011001111000000, 0b1111001111000000, 0b0000111111000000, 0b1100111111000000, 0b0011111111000000, 0b1111111111000000,
  0b0000000000110000, 0b1100000000110000, 0b0011000000110000, 0b1111000000110000, 0b0000110000110000, 0b1100110000110000, 0b0011110000110000, 0b1111110000110000,
  0b0000001100110000, 0b1100001100110000, 0b0011001100110000, 0b1111001100110000, 0b0000111100110000, 0b1100111100110000, 0b0011111100110000, 0b1111111100110000,
  0b0000000011110000, 0b1100000011110000, 0b0011000011110000, 0b1111000011110000, 0b0000110011110000, 0b1100110011110000, 0b0011110011110000, 0b1111110011110000,
  0b0000001111110000, 0b1100001111110000, 0b0011001111110000, 0b1111001111110000, 0b0000111111110000, 0b1100111111110000, 0b0011111111110000, 0b1111111111110000,
  0b0000000000001100, 0b1100000000001100, 0b0011000000001100, 0b1111000000001100, 0b0000110000001100, 0b1100110000001100, 0b0011110000001100, 0b1111110000001100,
  0b0000001100001100, 0b1100001100001100, 0b0011001100001100, 0b1111001100001100, 0b0000111100001100, 0b1100111100001100, 0b0011111100001100, 0b1111111100001100,
  0b0000000011001100, 0b1100000011001100, 0b0011000011001100, 0b1111000011001100, 0b0000110011001100, 0b1100110011001100, 0b0011110011001100, 0b1111110011001100,
  0b0000001111001100, 0b1100001111001100, 0b0011001111001100, 0b1111001111001100, 0b0000111111001100, 0b1100111111001100, 0b0011111111001100, 0b1111111111001100,
  0b0000000000111100, 0b1100000000111100, 0b0011000000111100, 0b1111000000111100, 0b0000110000111100, 0b1100110000111100, 0b0011110000111100, 0b1111110000111100,
  0b0000001100111100, 0b1100001100111100, 0b0011001100111100, 0b1111001100111100, 0b0000111100111100, 0b1100111100111100, 0b0011111100111100, 0b1111111100111100,
  0b0000000011111100, 0b1100000011111100, 0b0011000011111100, 0b1111000011111100, 0b0000110011111100, 0b1100110011111100, 0b0011110011111100, 0b1111110011111100,
  0b0000001111111100, 0b1100001111111100, 0b0011001111111100, 0b1111001111111100, 0b0000111111111100, 0b1100111111111100, 0b0011111111111100, 0b1111111111111100,
  0b0000000000000011, 0b1100000000000011, 0b0011000000000011, 0b1111000000000011, 0b0000110000000011, 0b1100110000000011, 0b0011110000000011, 0b1111110000000011,
  0b0000001100000011, 0b1100001100000011, 0b0011001100000011, 0b1111001100000011, 0b0000111100000011, 0b1100111100000011, 0b0011111100000011, 0b1111111100000011,
  0b0000000011000011, 0b1100000011000011, 0b0011000011000011, 0b1111000011000011, 0b0000110011000011, 0b1100110011000011, 0b0011110011000011, 0b1111110011000011,
  0b0000001111000011, 0b1100001111000011, 0b0011001111000011, 0b1111001111000011, 0b0000111111000011, 0b1100111111000011, 0b0011111111000011, 0b1111111111000011,
  0b0000000000110011, 0b1100000000110011, 0b0011000000110011, 0b1111000000110011, 0b0000110000110011, 0b1100110000110011, 0b0011110000110011, 0b1111110000110011,
  0b0000001100110011, 0b1100001100110011, 0b0011001100110011, 0b1111001100110011, 0b0000111100110011, 0b1100111100110011, 0b0011111100110011, 0b1111111100110011,
  0b0000000011110011, 0b1100000011110011, 0b0011000011110011, 0b1111000011110011, 0b0000110011110011, 0b1100110011110011, 0b0011110011110011, 0b1111110011110011,
  0b0000001111110011, 0b1100001111110011, 0b0011001111110011, 0b1111001111110011, 0b0000111111110011, 0b1100111111110011, 0b0011111111110011, 0b1111111111110011,
  0b0000000000001111, 0b1100000000001111, 0b0011000000001111, 0b1111000000001111, 0b0000110000001111, 0b1100110000001111, 0b0011110000001111, 0b1111110000001111,
  0b0000001100001111, 0b1100001100001111, 0b0011001100001111, 0b1111001100001111, 0b0000111100001111, 0b1100111100001111, 0b0011111100001111, 0b1111111100001111,
  0b0000000011001111, 0b1100000011001111, 0b0011000011001111, 0b1111000011001111, 0b0000110011001111, 0b1100110011001111, 0b0011110011001111, 0b1111110011001111,
  0b0000001111001111, 0b1100001111001111, 0b0011001111001111, 0b1111001111001111, 0b0000111111001111, 0b1100111111001111, 0b0011111111001111, 0b1111111111001111,
  0b0000000000111111, 0b1100000000111111, 0b0011000000111111, 0b1111000000111111, 0b0000110000111111, 0b1100110000111111, 0b0011110000111111, 0b1111110000111111,
  0b0000001100111111, 0b1100001100111111, 0b0011001100111111, 0b1111001100111111, 0b0000111100111111, 0b1100111100111111, 0b0011111100111111, 0b1111111100111111,
  0b0000000011111111, 0b1100000011111111, 0b0011000011111111, 0b1111000011111111, 0b0000110011111111, 0b1100110011111111, 0b0011110011111111, 0b1111110011111111,
  0b0000001111111111, 0b1100001111111111, 0b0011001111111111, 0b1111001111111111, 0b0000111111111111, 0b1100111111111111, 0b0011111111111111, 0b1111111111111111
};

uint16_t Table8BPP::interleaveBytes(byte hi, byte lo) {
  uint16_t hiWide = EXPANDED_REVERSED_BYTES[hi] & 0b1010101010101010;
  uint16_t loWide = EXPANDED_REVERSED_BYTES[lo] & 0b0101010101010101;
  return hiWide | loWide;
}

