#pragma once
class Image
{
public:
  Image(std::uint32_t x, std::uint32_t y);
  ~Image();
  void addSubImage(std::uint8_t * dataPtr, std::uint32_t sizeX, std::uint32_t sizeY);
  void setPixel(std::uint8_t data);

  std::uint32_t _posX;
  std::uint32_t _posY;

  std::uint32_t _sizeX;
  std::uint32_t _sizeY;
  std::vector<std::uint8_t> _buffer;
};

