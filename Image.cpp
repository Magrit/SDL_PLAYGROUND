#include "stdafx.h"
#include "Image.h"

Image::Image(std::uint32_t x, std::uint32_t y)
  : _posX(0)
  , _posY(0)
  , _sizeX(x)
  , _sizeY(y)
  , _buffer(x*y)
{
}

Image::~Image()
{
}


void Image::addSubImage(std::uint8_t *dataPtr, std::uint32_t sizeX, std::uint32_t sizeY)
{
  if (_posX + sizeX > _sizeX) {
    _posY += 256;
    _posX = 0;
  }
  if (_posY + sizeY > _sizeY) {
    std::cout << "IMAGE OVERFLOW, FIX THIS CODE!" << std::endl;
    return;
  }

  auto startOffset = _posY*_sizeX;
  for (std::uint32_t y = 0; y < sizeY; ++y) {
    std::copy(dataPtr + y*sizeX, dataPtr + y*sizeX + sizeX, _buffer.begin() + startOffset + y*_sizeX + _posX);
  }
  _posX += sizeX;
}

void Image::setPixel(std::uint8_t data)
{
}
