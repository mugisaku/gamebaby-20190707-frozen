#include"libge/ge.hpp"




constexpr int  rect_size = 16;




color_holder::
color_holder(int  size, void  (*callback)(color_holder&,gbstd::color)) noexcept:
m_colors(size),
m_callback(callback)
{
  set_content_width(rect_size+(gbstd::g_font_width*3));
  set_content_height(gbstd::g_font_height*m_colors.size());

    for(auto&  e: m_colors)
    {
      e = gbstd::color(0,0,0);
    }
}


color_holder::
color_holder(std::initializer_list<gbstd::color>  colors, void  (*callback)(color_holder&,gbstd::color)) noexcept:
m_colors(colors),
m_callback(callback)
{
  set_content_width(rect_size+(gbstd::g_font_width*3));
  set_content_height(gbstd::g_font_height*m_colors.size());
}




void
color_holder::
set_color(gbstd::color  color) noexcept
{
  auto&  dst = m_colors[m_index];

    if(dst != color)
    {
      dst = color;

      request_redraw();
    }
}


void
color_holder::
render(const gbstd::canvas&  cv) noexcept
{
  cv.fill(get_window()->get_background_color());

  int  y = 0;

    for(auto&  color: m_colors)
    {
      cv.draw_rectangle(gbstd::colors::white,0,y  ,rect_size  ,rect_size  );
      cv.fill_rectangle(color               ,1,y+1,rect_size-2,rect_size-2);


      gbstd::string_form  sf("%d%d%d",color.get_r7(),color.get_g7(),color.get_b7());

      static const gbstd::character_color  tc(gbstd::colors::white);

      cv.draw_string(tc,sf(),rect_size,y);

      y += rect_size;
    }



  cv.draw_rectangle(gbstd::colors::white,0,rect_size*m_index,get_content_width(),rect_size);
}


void
color_holder::
do_on_mouse_act(gbstd::point  mouse_pos) noexcept
{
    if(gbstd::g_input.test_mouse_left())
    {
      m_index = mouse_pos.y/rect_size;

        if(m_callback)
        {
          m_callback(*this,get_color());
        }


      request_redraw();
    }
}





