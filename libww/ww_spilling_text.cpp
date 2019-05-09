#include"libww/ww_spilling_text.hpp"




namespace ww{




spilling_text&
spilling_text::
reset(gbstd::real_point  bottom_pos, int  offset) noexcept
{
  m_bottom_pos = bottom_pos;
  m_pos        = bottom_pos;

  m_pos.y += offset;

  m_y_vector = -8;

  return *this;
}


void
spilling_text::
tick(spilling_text&  spltxt) noexcept
{
  spltxt.m_pos.y += spltxt.m_y_vector       ;
                    spltxt.m_y_vector += 2.0;

    if(spltxt.m_pos.y >= spltxt.m_bottom_pos.y+16)
    {
      spltxt.m_y_vector /= 4;

        if(spltxt.m_y_vector > 1)
        {
          spltxt.m_y_vector = -spltxt.m_y_vector;
        }

      else
        {
          spltxt.m_text.clear();
        }
    }
}


void
spilling_text::
draw(const gbstd::canvas&  cv, spilling_text&  spltxt) noexcept
{
  auto  cc = gbstd::character_color(spltxt.m_color,gbstd::colors::black,gbstd::colors::black);

  cv.draw_string_safely(cc,spltxt.m_text.data(),spltxt.m_pos.x,
                                                spltxt.m_pos.y);
}




}




