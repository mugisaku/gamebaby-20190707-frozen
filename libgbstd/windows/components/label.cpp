#include"libgbstd/windows/component.hpp"



namespace gbstd{




color
label::
m_default_color(colors::white);


label::
label(const char*  s) noexcept:
widget(0,16+4)
{
  set_text(s);

  set_color(m_default_color);
}


label::
label(const char16_t*  s) noexcept:
widget(0,16+4)
{
  set_text(s);

  set_color(m_default_color);
}


label::
label(const char16_t*  s, gbstd::color  color) noexcept:
widget(0,16+4)
{
  set_text(s);

  set_color(color);
}




void
label::
set_text(const char16_t*  s) noexcept
{
  modify_text(s);

  set_content_width((8*m_text.size())+4);

  request_reform();
}


void
label::
set_text(const char*  s) noexcept
{
  modify_text(s);

  set_content_width((8*m_text.size())+4);

  request_reform();
}


void
label::
modify_text(const char16_t*  s) noexcept
{
  m_text = s;

  request_redraw();
}


void
label::
modify_text(const char*  s) noexcept
{
  m_text = make_u16string(s);

  request_redraw();
}


void
label::
set_color(color  color) noexcept
{
  m_color = color;

  request_redraw();
}


void
label::
render(const canvas&  cv) noexcept
{
  cv.draw_string(m_color,m_text.data(),2,2);
}




}




