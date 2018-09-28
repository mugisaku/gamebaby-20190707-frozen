#include"libgbstd/character.hpp"



namespace gbstd{




point&
point::
operator+=(point  rhs) noexcept
{
  x += rhs.x;
  y += rhs.y;

  return *this;
}


point&
point::
operator-=(point  rhs) noexcept
{
  x -= rhs.x;
  y -= rhs.y;

  return *this;
}


point&
point::
operator*=(point  rhs) noexcept
{
  x *= rhs.x;
  y *= rhs.y;

  return *this;
}


point&
point::
operator/=(point  rhs) noexcept
{
  x /= rhs.x;
  y /= rhs.y;

  return *this;
}


point&
point::
operator%=(point  rhs) noexcept
{
  x %= rhs.x;
  y %= rhs.y;

  return *this;
}




}





