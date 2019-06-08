#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{




cursor&
cursor::
reset(view&  v) noexcept
{
  m_view = &v;

  m_position = {0,0};

  return *this;
}


cursor&
cursor::
set_x(int  n) noexcept
{
  m_position.x = std::max(0,std::min(n,m_view->get_width()-1));

  return *this;
}


cursor&
cursor::
add_x(int  n) noexcept
{
  m_position.x = std::max(0,std::min(m_position.x+n,m_view->get_width()-1));

  return *this;
}


cursor&
cursor::
set_y(int  n) noexcept
{
  m_position.y = std::max(0,std::min(n,m_view->get_height()-1));

  return *this;
}

cursor&
cursor::
add_y(int  n) noexcept
{
  m_position.y = std::max(0,std::min(m_position.y+n,m_view->get_height()-1));

  return *this;
}




}}




