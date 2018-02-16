#include "Table2BPP.h"
#include "ROM.h"
#include "Table8BPP.h"

Table8BPP Table2BPP::expand() const {
  return Table8BPP(*this);
}

Table2BPP::Table2BPP(const ByteArray& romData) : data(romData) {
  //nop
}

Table2BPP::Table2BPP(const Table8BPP& expanded) {
  throw std::exception("Table2BPP::Table2BPP() - Not yet implemented."); //~~_

  constexpr size_t SLICE_LENGTH = 8;
  ByteArray slice(SLICE_LENGTH);

  
  for(auto iter = expanded.data.rbegin(); iter != expanded.data.rend(); iter += SLICE_LENGTH) {
    std::copy(iter, iter + SLICE_LENGTH, slice.begin());

    uint16_t compacted = compact(slice);

    std::pair<byte, byte> bytes = deinterleave(compacted);

    data.push_back(bytes.first);
    data.push_back(bytes.second);
  }
}

std::pair<byte,byte> Table2BPP::deinterleave(uint16_t duo) {
  return std::pair<byte,byte>(); //~~_
}

uint16_t Table2BPP::compact(const ByteArray& texels) {
  return uint16_t(); //~~_
}


