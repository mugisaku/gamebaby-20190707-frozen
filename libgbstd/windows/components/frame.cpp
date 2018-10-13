#include"libgbstd/windows/component.hpp"



namespace gbstd{




constexpr int  margin = 16;


character_color
frame::
m_default_character_color(colors::white);


color
frame::
m_default_line_color = colors::yellow;




frame::
frame(const char*  text, widget*  wg, int  x, int  y) noexcept:
m_character_color(m_default_character_color),
m_line_color(m_default_line_color)
{
  m_container = new widget;
  m_label     = new label;

  m_container->set_left_padding(margin/2);
  m_container->set_right_padding(margin/2);
//  m_container->set_top_padding(margin);
  m_container->set_bottom_padding(margin/2);

  append_column_child({m_label,m_container});

  m_label->set_color(m_default_character_color);

  set_text(text);


  insert_content(wg,x,y);
}




void
frame::
insert_content(widget*  wg, int  x, int  y) noexcept
{
    if(wg)
    {
      m_container->append_child(wg,x,y);
    }
}


void
frame::
set_text(const char*  s) noexcept
{
  m_label->set_text(s);

  request_reform();
}


void
frame::
set_line_color(color  new_color) noexcept
{
  m_line_color = new_color;

  request_redraw();
}


void
frame::
render(const canvas&  cv) noexcept
{
  cv.draw_rectangle(m_line_color,0,0,get_width(),get_height());
}




}




