#include "ROM.h"
#include "Utility.h"
#include <algorithm>
#include "SNESTable.h"

ROM::ROM(const std::string & filename) : data(Utility::loadFile(filename)), tableOffset(findTableOffset()) {
  if(data.size() != 2 * MB) { throw std::runtime_error("ROM::ROM() - Invalid ROM file length."); }
}

SNESTable ROM::extractTable() const {
  auto head = data.begin() + tableOffset;
  auto tail = head + NATIVE_TABLE_LENGTH;

  return SNESTable(ByteArray(head, tail));
}

void ROM::injectTable(const SNESTable& table) {
  auto head = data.begin() + tableOffset;
  std::copy(table.data.begin(), table.data.end(), head);
}

void ROM::save(const std::string& filename) {
  Utility::saveFile(filename, data);
}

size_t ROM::findTableOffset() {
  constexpr byte OPENING[11] = { 0xA9, 0x02, 0x8D, 0x01, 0x21, 0xA9, 0x80, 0x8D, 0x15, 0x21, 0xA9 };
  //0xXX,
  constexpr byte MIDDLE[11] = { 0x85, 0x02, 0xC2, 0x30, 0xA9, 0x00, 0x70, 0x8D, 0x16, 0x21, 0xA9 };
  //0xZZ, 0xYY
  constexpr byte TAIL[2] = { 0x85, 0x00 };

  auto iter = std::search(data.begin() + MB, data.end(), std::begin(OPENING), std::end(OPENING));

  while(iter != data.end()) {
    if(std::equal(std::begin(MIDDLE), std::end(MIDDLE), iter + sizeof(OPENING) + 1)) {
      iter += sizeof(OPENING);
      byte snesBank = *iter++;

      iter += sizeof(MIDDLE);
      uint16_t snesAddr = *(iter + 1);
      snesAddr <<= 8;
      snesAddr |= *iter;

      return translateSNESAddress(snesBank, snesAddr);
    }
    else {
      iter = std::search(iter + 1, data.end(), std::begin(OPENING), std::end(OPENING));
    }
  }

  throw std::runtime_error("ROM::findTableOffset() - Code span not found. (This ROM is not compatible.)");
}

size_t ROM::translateSNESAddress(byte bank, uint16_t bankAddr) {
  return (bank << 15) | (bankAddr & 0x7FFF);
}

