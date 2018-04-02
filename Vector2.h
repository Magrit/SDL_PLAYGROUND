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
  

  template <typename T2>
  inline XVector2<T> operator+(const XVector2<T2>& rhs) const { return { x + T(rhs.x), y + T(rhs.y) }; }
   
  template <typename T2>
  inline XVector2<T> operator-(const XVector2<T2>& rhs) const { return { x - T(rhs.x), y - T(rhs.y) }; }
  

  T x, y;
};


typedef XVector2<float> Vector2f;
typedef XVector2<std::int32_t> Vector2i;
typedef XVector2<std::uint32_t> Vector2u;