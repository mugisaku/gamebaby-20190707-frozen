#include"libgbstd/character.hpp"




namespace gbstd{




text_cursor&
text_cursor::
operator++() noexcept
{
    if(++m_point.x >= m_screen->get_width())
    {
      m_point.x = 0;

        if(m_point.y < (m_screen->get_height()-1))
        {
          ++m_point.y;
        }
    }


  return *this;
}


text_cursor
text_cursor::
operator++(int) noexcept
{
  auto  tmp = *this;

  ++(*this);

  return tmp;
}


text_cursor&
text_cursor::
operator+=(point  pt) noexcept
{
  m_point += pt;

  return *this;
}




}




