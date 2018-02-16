#include "SNESTable.h"
#include "PNGTable.h"

PNGTable SNESTable::expand() const {
  return PNGTable(*this);
}

SNESTable::SNESTable(const ByteArray& romData) : data(romData) {
  //nop
}

SNESTable::SNESTable(const PNGTable& expanded) {
  constexpr size_t SLICE_LENGTH = TEXELS_PER_BYTE * 2;
  
  for(auto iter = expanded.data.rbegin(); iter != expanded.data.rend(); iter += SLICE_LENGTH) {
    uint16_t compacted = compact(iter, iter + SLICE_LENGTH);

    std::pair<byte, byte> bytes = deinterleave(compacted);

    data.push_back(bytes.first);
    data.push_back(bytes.second);
  }
}

std::pair<byte,byte> SNESTable::deinterleave(uint16_t duo) {
  uint16_t hiWide = duo & 0b1010101010101010;
  hiWide |= (hiWide >> 1);
  uint16_t loWide = duo & 0b0101010101010101;
  loWide |= (loWide << 1);

  auto hiIter = std::find(std::begin(PNGTable::EXPANDED_REVERSED_BYTES), std::end(PNGTable::EXPANDED_REVERSED_BYTES), hiWide);
  auto loIter = std::find(std::begin(PNGTable::EXPANDED_REVERSED_BYTES), std::end(PNGTable::EXPANDED_REVERSED_BYTES), loWide);

  ptrdiff_t hi = hiIter - std::begin(PNGTable::EXPANDED_REVERSED_BYTES);
  ptrdiff_t lo = loIter - std::begin(PNGTable::EXPANDED_REVERSED_BYTES);

  return std::make_pair((byte)hi, (byte)lo);
}

uint16_t SNESTable::compact(ByteArray::const_reverse_iterator head, ByteArray::const_reverse_iterator tail) {
  uint16_t duo = 0;

  for(; head != tail; head++) {
    duo <<= 2;
    duo |= *head;
  }

  return duo;
}


