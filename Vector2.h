#pragma once
#include "stdafx.h"

template <typename T> 
class XVector2
{
public:
  XVector2<T>() 
  : x(0.0f)
  , y(0.0f)
  {}
  XVector2<T>(T inX, T inY)
    : x(inX)
    , y(inY)
  {}

  T x, y;
};


typedef XVector2<float> Vector2f;
typedef XVector2<std::int32_t> Vector2i;
typedef XVector2<std::uint32_t> Vector2u;