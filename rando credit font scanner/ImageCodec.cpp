#include "ImageCodec.h"
#include "Bitmap.h"

#pragma comment(lib, "windowscodecs.lib")

CComPtr<IWICImagingFactory> ImageCodec::factory;
const WICPixelFormatGUID ImageCodec::format = GUID_WICPixelFormat32bppBGRA;

std::tuple<UINT, UINT, std::vector<uint32_t>> ImageCodec::loadFromFile(const std::string& fname) {
  if(!factory.p) { initialize(); }

  CComPtr<IWICBitmapDecoder> decoder;
  HRESULT hr = factory->CreateDecoderFromFilename(expandString(fname).data(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to open input file."); }

  CComPtr<IWICBitmapFrameDecode> bitmapFrame;
  hr = decoder->GetFrame(0, &bitmapFrame);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to retrieve image frame."); }

  CComPtr<IWICFormatConverter> converter;
  factory->CreateFormatConverter(&converter);
  hr = converter->Initialize(bitmapFrame, format, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to initialize converter."); }

  UINT width, height;
  std::vector<uint32_t> data;

  hr = bitmapFrame->GetSize(&width, &height);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to retrieve image dimensions."); }

  data.resize(width * height);

  const size_t STRIDE = width * BYTES_PER_PIXEL;
  const size_t LENGTH = data.size() * BYTES_PER_PIXEL;

  hr = converter->CopyPixels(nullptr, STRIDE, LENGTH, (byte*)data.data());
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::loadFromFile() - Failed to read pixels from image file."); }

  return std::make_tuple(width, height, data);
}

void ImageCodec::saveToFile(const std::string& fname, TextureData texture) {
  if(!factory.p) { initialize(); }

  UINT width;
  UINT height;
  std::vector<uint32_t> data;
  std::tie(width, height, data) = texture;

  const size_t STRIDE = width * BYTES_PER_PIXEL;
  const size_t LENGTH = data.size() * BYTES_PER_PIXEL;

  CComPtr<IWICBitmapEncoder> encoder;
  HRESULT hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to create encoder."); }

  CComPtr<IWICStream> stream;
  hr = factory->CreateStream(&stream);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to create output stream."); }

  hr = stream->InitializeFromFilename(expandString(fname).data(), GENERIC_WRITE);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to attach output stream to file."); }

  hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to initialize encoder."); }

  CComPtr<IWICBitmapFrameEncode> frame;
  CComPtr<IPropertyBag2> properties;
  hr = encoder->CreateNewFrame(&frame, &properties);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to create frame."); }
  hr = frame->Initialize(properties);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to initialize frame."); }

  hr = frame->SetSize(width, height);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to set frame size."); }

  auto fmt = format;
  hr = frame->SetPixelFormat(&fmt);
  if(FAILED(hr) || !IsEqualGUID(fmt, format)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to set frame format."); }

  hr = frame->WritePixels(height, STRIDE, LENGTH, (byte*)data.data());
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to write to frame."); }
  hr = frame->Commit();
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to commit frame."); }
  hr = encoder->Commit();
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::saveFromFile() - Failed to commit encode."); }
}

void ImageCodec::savePackedToFile(const std::string& fname, const ByteArray& data) {
  if(!factory.p) { initialize(); }

  UINT width = 144; //~~@
  UINT height = 272; //~~@

  const size_t STRIDE = width;
  const size_t LENGTH = STRIDE * height;

  _ASSERT(LENGTH == data.size());

  CComPtr<IWICBitmapEncoder> encoder;
  HRESULT hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to create encoder."); }

  CComPtr<IWICStream> stream;
  hr = factory->CreateStream(&stream);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to create output stream."); }

  hr = stream->InitializeFromFilename(expandString(fname).data(), GENERIC_WRITE);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to attach output stream to file."); }

  hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to initialize encoder."); }

  CComPtr<IWICBitmapFrameEncode> frame;
  CComPtr<IPropertyBag2> properties;
  hr = encoder->CreateNewFrame(&frame, &properties);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to create frame."); }
  hr = frame->Initialize(properties);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to initialize frame."); }

  hr = frame->SetSize(width, height);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to set frame size."); }

  auto fmt = GUID_WICPixelFormat8bppIndexed;
  hr = frame->SetPixelFormat(&fmt);
  if(FAILED(hr) || !IsEqualGUID(fmt, GUID_WICPixelFormat8bppIndexed)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to set frame format."); }

  CComPtr<IWICPalette> palette;
  hr = factory->CreatePalette(&palette);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to create palette."); }

  WICColor colors[5] = {
    0xFF000000, //BLACK
    0xFFFFFFFF, //WHITE
    0xFF0000FF, //BLUE
    0xFFFF0000, //RED
    0xFF00FF00  //GREEN
  };

  palette->InitializeCustom(colors, 5);

  hr = frame->SetPalette(palette);
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to set palette."); }

  ByteArray temp = data;

  hr = frame->WritePixels(height, STRIDE, LENGTH, temp.data());
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to write to frame."); }
  hr = frame->Commit();
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to commit frame."); }
  hr = encoder->Commit();
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::savePackedToFile() - Failed to commit encode."); }

}

std::wstring ImageCodec::expandString(const std::string& in) {
  size_t dumbNum;
  std::wstring out;
  out.resize(in.size() + 1);
  mbstowcs_s(&dumbNum, &out[0], out.size(), in.data(), in.size());
  return out;
}

void ImageCodec::initialize() {
  CoInitialize(nullptr);
  HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
  if(FAILED(hr)) { throw std::runtime_error("ImageCodec::ImageCodec() - Failed to create WIC Factory instance."); }
}



