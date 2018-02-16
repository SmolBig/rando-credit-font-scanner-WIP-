#pragma once
#include <wincodec.h>
#include <atlbase.h>
#include <vector>
#include <string>
#include <tuple>
#include "Utility.h"

using TextureData = std::tuple<UINT, UINT, ByteArray>;

class ImageCodec {
public:
  static TextureData loadFromFile(const std::string& fname);
  static void saveToFile(const std::string& fname, const TextureData& texture);

private:
  static CComPtr<IWICImagingFactory> factory;
  static const WICPixelFormatGUID format;
  static constexpr size_t BYTES_PER_PIXEL = 1;

  static void initialize();

};

