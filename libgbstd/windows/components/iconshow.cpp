#include"libgbstd/windows/component.hpp"



namespace gbstd{




void
iconshow::
set_index(int  i) noexcept
{
  m_index = i;

  request_redraw();
}


void
iconshow::
do_on_mouse_leave() noexcept
{
  m_whether_touched = false;

    if(m_callback)
    {
      m_callback({*this,iconshow_event::kind::leave});
    }
}


void
iconshow::
do_on_mouse_act(point  mouse_pos) noexcept
{
    if(m_callback)
    {
        if(g_input.test_mouse_left())
        {
          m_whether_touched = true;
        }

      else
        if(m_whether_touched)
        {
          m_whether_touched = false;

            if(m_callback)
            {
              m_callback({*this,iconshow_event::kind::click});
            }
        }
    }
}


void
iconshow::
render(const canvas&  cv) noexcept
{
    if(m_icons.size())
    {
      auto&  icon = *m_icons[m_index];

        for(auto  y = 0;  y < g_icon_size;  ++y){
        for(auto  x = 0;  x < g_icon_size;  ++x){
          cv.draw_dot(icon.get_color(x,y),x,y);
        }}
    }
}




}




