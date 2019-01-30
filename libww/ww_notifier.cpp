#include"libww/ww.hpp"




namespace ww{




notifier::
notifier(gbstd::point  start_pos, const char*  s) noexcept:
m_next_time(0),
m_y_vector(-8),
m_x_pos(start_pos.x),
m_y_pos(start_pos.y),
m_start_pos(start_pos),
m_text(s)
{
}


void
notifier::
step() noexcept
{
    if(gbstd::g_time >= m_next_time)
    {
      m_y_pos += m_y_vector       ;
                 m_y_vector += 2.0;

        if(m_y_pos >= m_start_pos.y+16)
        {
          m_text.clear();
        }


      m_next_time = gbstd::g_time+80;
    }
}


void
notifier::
render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept
{
  cv.draw_string(gbstd::colors::white,m_text.data(),offset.x+static_cast<int>(m_x_pos),
                                                    offset.y+static_cast<int>(m_y_pos));
}




}




