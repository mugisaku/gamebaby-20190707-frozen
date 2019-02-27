#include"libww/ww_notifier.hpp"




namespace ww{




notifier::
notifier(gbstd::point  offset, gbstd::point  start_pos, const char*  s) noexcept:
m_y_vector(-8),
m_x_pos(start_pos.x),
m_y_pos(start_pos.y),
m_offset(offset),
m_start_pos(start_pos),
m_text(s)
{
}


void
notifier::
step() noexcept
{
  m_y_pos += m_y_vector       ;
             m_y_vector += 2.0;

    if(m_y_pos >= m_start_pos.y+16)
    {
      m_text.clear();
    }
}


void
notifier::
render(const gbstd::canvas&  cv) const noexcept
{
  cv.draw_string(gbstd::colors::white,m_text.data(),m_offset.x+static_cast<int>(m_x_pos),
                                                    m_offset.y+static_cast<int>(m_y_pos));
}




}




