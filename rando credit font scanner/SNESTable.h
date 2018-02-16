#pragma once
#include "Utility.h"
#include <tuple>

class PNGTable;
class ROM;

class SNESTable {
public:
  PNGTable expand() const;

private:
  ByteArray data;
  static const size_t LENGTH = 0x2000;

  friend class ROM;
  SNESTable(const ByteArray& romData);

  friend class PNGTable;
  SNESTable(const PNGTable& expanded);

  static constexpr size_t TEXELS_PER_BYTE = 4;

  static std::pair<byte, byte> deinterleave(uint16_t duo);
  static uint16_t compact(ByteArray::const_reverse_iterator head, ByteArray::const_reverse_iterator tail);

};

