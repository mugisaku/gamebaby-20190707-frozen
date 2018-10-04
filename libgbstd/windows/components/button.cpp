#include"libgbstd/windows/component.hpp"



namespace gbstd{




constexpr int  margin = 4;


button::
button(widget*  target, void  (*callback)(button_event)) noexcept:
m_callback(callback)
{
  be_autonomous();

  m_container = new widget;

  m_container->append_child(target,0,0);

  append_child(m_container,0,0);
}




void
button::
do_on_mouse_leave() noexcept
{
    if(is_pressed())
    {
      m_state = state::released;

        if(m_callback)
        {
          m_callback({*this,0});
        }


      request_redraw();
    }
}


void
button::
do_on_mouse_act(point  mouse_pos) noexcept
{
    if(is_released())
    {
        if(g_input.test_mouse_left())
        {
          m_state = state::pressed;

            if(m_callback)
            {
              m_callback({*this,1});
            }


          request_redraw();
        }
    }

  else
    {
        if(!g_input.test_mouse_left())
        {
          do_on_mouse_leave();
        }
    }
}




void
button::
render(const canvas&  cv) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

    if(is_pressed())
    {
      cv.draw_hline(colors::black,0,0,w);
      cv.draw_hline(colors::white,0,h-1,w);
      cv.draw_vline(colors::black,0,0,h);
      cv.draw_vline(colors::white,w-1,0,h);

      cv.fill_rectangle(color(5,5,5),1,1,w-2,h-2);
    }

  else
    {
      cv.draw_hline(colors::white,0,0,w);
      cv.draw_hline(colors::black,0,h-1,w);
      cv.draw_vline(colors::white,0,0,h);
      cv.draw_vline(colors::black,w-1,0,h);

      cv.fill_rectangle(color(6,6,6),1,1,w-2,h-2);
    }
}


void
button::
print(int  indent) const noexcept
{
}




}




