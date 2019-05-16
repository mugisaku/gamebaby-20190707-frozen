#include"libww/ww_string_display.hpp"




namespace ww{




display_string&
display_string::
set_color(gbstd::color  color) noexcept
{
  m_color = color;

  return *this;
}


display_string&
display_string::
set_string(std::u16string_view  sv) noexcept
{
  char16_t*  dst     = m_buffer;
  char16_t*  dst_end = m_buffer+(m_buffer_length-1);

  const char16_t*  src = sv.data();

  m_length = 0;

    while(dst != dst_end)
    {
      *dst++ = *src++;

      ++m_length;
    }


  *dst = 0;

  return *this;
}




void
string_display::
draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, string_display&  strdsp) noexcept
{
    if(strdsp.m_pointer)
    {
      auto  sv = strdsp.m_pointer->get_string_view();

      auto  cc = gbstd::character_color(strdsp.m_pointer->get_color(),gbstd::colors::black,gbstd::colors::black);

      cv.draw_string_safely(cc,sv,strdsp.m_pos.x,
                                  strdsp.m_pos.y);
    }
}




}




