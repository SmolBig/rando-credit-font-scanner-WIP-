#include "ImageCodec.h"
#include "Bitmap.h"

#pragma comment(lib, "windowscodecs.lib")

CComPtr<IWICImagingFactory> ImageCodec::factory;
const WICPixelFormatGUID ImageCodec::format = GUID_WICPixelFormat8bppIndexed;

TextureData ImageCodec::loadFromFile(const std::string& fname) {
  if(!factory.p) { initialize(); }

  CComPtr<IWICBitmapDecoder> decoder;
  HRESULT hr = factory->CreateDecoderFromFilename(Utility::expandString(fname).data(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to open input file."); }

  CComPtr<IWICBitmapFrameDecode> bitmapFrame;
  hr = decoder->GetFrame(0, &bitmapFrame);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to retrieve image frame."); }

  CComPtr<IWICFormatConverter> converter;
  factory->CreateFormatConverter(&converter);
  hr = converter->Initialize(bitmapFrame, format, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to initialize converter."); }

  UINT width, height;
  ByteArray data;

  hr = bitmapFrame->GetSize(&width, &height);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to retrieve image dimensions."); }

  data.resize(width * height);

  const size_t STRIDE = width * BYTES_PER_PIXEL;
  const size_t LENGTH = data.size() * BYTES_PER_PIXEL;

  hr = converter->CopyPixels(nullptr, STRIDE, LENGTH, (byte*)data.data());
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to read pixels from image file."); }

  return std::make_tuple(width, height, data);
}

void ImageCodec::saveToFile(const std::string& fname, const TextureData& texture) {
  if(!factory.p) { initialize(); }

  UINT width;
  UINT height;
  ByteArray data;
  std::tie(width, height, data) = texture;

  const size_t STRIDE = width * BYTES_PER_PIXEL;
  const size_t LENGTH = data.size() * BYTES_PER_PIXEL;

  CComPtr<IWICBitmapEncoder> encoder;
  HRESULT hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to create encoder."); }

  CComPtr<IWICStream> stream;
  hr = factory->CreateStream(&stream);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to create output stream."); }

  hr = stream->InitializeFromFilename(Utility::expandString(fname).data(), GENERIC_WRITE);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to attach output stream to file."); }

  hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to initialize encoder."); }

  CComPtr<IWICBitmapFrameEncode> frame;
  CComPtr<IPropertyBag2> properties;
  hr = encoder->CreateNewFrame(&frame, &properties);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to create frame."); }
  hr = frame->Initialize(properties);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to initialize frame."); }

  hr = frame->SetSize(width, height);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to set frame size."); }

  auto fmt = format;
  hr = frame->SetPixelFormat(&fmt);
  if(FAILED(hr) || !IsEqualGUID(fmt, format)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to set frame format."); }

  CComPtr<IWICPalette> palette;
  hr = factory->CreatePalette(&palette);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to create palette."); }

  std::vector<WICColor> colors(std::begin((Bitmap::PALETTE)), std::end((Bitmap::PALETTE)));
  hr = palette->InitializeCustom(colors.data(), colors.size());
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to initialize palette."); }
  hr = frame->SetPalette(palette);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to set palette."); }

  hr = frame->WritePixels(height, STRIDE, LENGTH, data.data());
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to write to frame."); }
  hr = frame->Commit();
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to commit frame."); }
  hr = encoder->Commit();
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveToFile() - Failed to commit encode."); }
}

void ImageCodec::initialize() {
  CoInitialize(nullptr);
  HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::ImageCodec() - Failed to create WIC Factory instance."); }
}



