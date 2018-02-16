#pragma once
#include "Utility.h"
#include <string>

class Table2BPP;

class ROM {
public:
  ROM(const std::string& filename);

  Table2BPP extractTable() const;
  void injectTable(const Table2BPP& table);
  void save(const std::string& filename);

private:
  ByteArray data;
  size_t tableOffset;

  size_t findTableOffset();
  static size_t translateSNESAddress(byte bank, uint16_t bankAddr);

  static constexpr size_t NATIVE_TABLE_LENGTH = 0x2000;

};

