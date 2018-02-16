#include "Utility.h"
#include <fstream>

ByteArray Utility::loadFile(const std::string& fname) {
  std::ifstream file(fname, std::ifstream::binary);
  if(!file) { throw std::runtime_error("loadFile() - File could not be opened."); }

  file.seekg(0, file.end);
  int len = (int)file.tellg();
  file.seekg(0, file.beg);

  ByteArray data;
  data.resize(len);
  file.read((char*)&data[0], len);

  return data;
}

void Utility::saveFile(const std::string& fname, const ByteArray& data) {
  std::ofstream file(fname, std::ofstream::binary);
  if(!file) { throw std::runtime_error("saveFile() - File could not be opened."); }
  file.write((const char*)data.data(), data.size());
}

