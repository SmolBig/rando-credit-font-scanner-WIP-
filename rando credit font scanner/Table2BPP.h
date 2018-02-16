#pragma once
#include "Utility.h"
#include <tuple>

class Table8BPP;
class ROM;

class Table2BPP {
public:
  Table8BPP expand() const;

private:
  ByteArray data;
  static const size_t LENGTH = 0x2000;

  friend class ROM;
  Table2BPP(const ByteArray& romData);

  friend class Table8BPP;
  Table2BPP(const Table8BPP& expanded);

  static constexpr size_t TEXELS_PER_BYTE = 4;

  std::pair<byte, byte> deinterleave(uint16_t duo);
  uint16_t compact(const ByteArray& texels);

};

