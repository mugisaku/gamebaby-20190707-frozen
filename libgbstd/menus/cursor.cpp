#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{




cursor&
cursor::
reset(view&  v) noexcept
{
  m_view = &v;

  m_point = {0,0};

  return *this;
}


cursor&
cursor::
set_x(int  n) noexcept
{
  m_point.x = std::max(0,std::min(n,m_view->get_width()-1));

  return *this;
}


cursor&
cursor::
add_x(int  n) noexcept
{
  m_point.x = std::max(0,std::min(m_point.x+n,m_view->get_width()-1));

  return *this;
}


cursor&
cursor::
set_y(int  n) noexcept
{
  m_point.y = std::max(0,std::min(n,m_view->get_height()-1));

  return *this;
}

cursor&
cursor::
add_y(int  n) noexcept
{
  m_point.y = std::max(0,std::min(m_point.y+n,m_view->get_height()-1));

  return *this;
}




}}




