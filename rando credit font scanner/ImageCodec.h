#pragma once
#include <wincodec.h>
#include <atlbase.h>
#include <vector>
#include <string>
#include <tuple>
#include "Utility.h"

using TextureData = std::tuple<UINT, UINT, std::vector<uint32_t>>;

class ImageCodec {
public:
  static TextureData loadFromFile(const std::string& fname);
  static void saveToFile(const std::string& fname, TextureData texture);

  static void savePackedToFile(const std::string& fname, const ByteArray& table);

private:
  static CComPtr<IWICImagingFactory> factory;
  static const WICPixelFormatGUID format;
  static constexpr size_t BYTES_PER_PIXEL = 4;

  static std::wstring expandString(const std::string& in);
  static void initialize();

};

