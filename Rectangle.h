#pragma once
#include "Vector2.h"

template <typename T>
class XRectangle
{
public:
  XRectangle<T>() {}

  XRectangle<T>(const XVector2<T> &min, const XVector2<T> &max)
    : _min(min)
    , _max(max)
  {}

  inline XVector2<T> getLowerLeft() { return _min; }
  inline XVector2<T> getLowerRight() { return { _max.x, _min.y }; }
  inline XVector2<T> getUpperLeft() { return { _min.x, _max.y }; }
  inline XVector2<T> getUpperRight() { return _max; }


  XVector2<T> _min;
  XVector2<T> _max;
};


typedef XRectangle<float> Rectanglef;
typedef XRectangle<std::int32_t> Rectanglei;
typedef XRectangle<std::uint32_t> Rectangleu;