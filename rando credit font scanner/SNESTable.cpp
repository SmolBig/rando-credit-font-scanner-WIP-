#include "SNESTable.h"
#include "PNGTable.h"

PNGTable SNESTable::expand() const {
  return PNGTable(*this);
}

SNESTable::SNESTable(const ByteArray& romData) : data(romData) {
  //nop
}

SNESTable::SNESTable(const PNGTable& expanded) {
  throw std::exception("SNESTable::SNESTable() - Not yet implemented."); //~~_

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

std::pair<byte,byte> SNESTable::deinterleave(uint16_t duo) {
  throw std::exception("SNESTable::deinterleave() - Not yet implemented."); //~~_
}

uint16_t SNESTable::compact(const ByteArray& texels) {
  throw std::exception("SNESTable::compact() - Not yet implemented."); //~~_
}


