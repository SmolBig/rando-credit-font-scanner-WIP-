#pragma once
#include <string>
#include <vector>

constexpr size_t MB = 0x100000;

using byte = uint8_t;
using ByteArray = std::vector<byte>;

union uColor {
  uint32_t value;
  struct { byte b, g, r, a; };
  byte ary[4];
};

namespace Utility {
  ByteArray loadFile(const std::string& fname);
  void saveFile(const std::string& fname, const ByteArray& data);
  std::wstring expandString(const std::string& in);
}
