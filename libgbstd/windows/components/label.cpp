#include"libgbstd/windows/component.hpp"



namespace gbstd{




character_color
label::
m_default_color(colors::null,colors::white,colors::null,colors::null);


label::
label(const char*  s) noexcept:
widget(0,16+4)
{
  set_text(s);

  set_color(m_default_color);
}


label::
label(const char*  s, const character_color&  color) noexcept:
widget(0,16+4)
{
  set_text(s);

  set_color(color);
}


label::
label(const char16_t*  s) noexcept:
widget(0,16+4)
{
  set_text(s);

  set_color(m_default_color);
}


label::
label(const char16_t*  s, const character_color&  color) noexcept:
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

  set_width((8*m_text.size())+4);

  request_reform();
}


void
label::
set_text(const char*  s) noexcept
{
  modify_text(s);

  set_width((8*m_text.size())+4);

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
  utf8_decoder  dec(s);

  m_text.clear();

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      m_text += c;
    }


  request_redraw();
}


void
label::
set_color(const character_color&  color) noexcept
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




