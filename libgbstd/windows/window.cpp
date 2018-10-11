#include"libgbstd/window.hpp"



namespace gbstd{




window::
window(const char*  name) noexcept:
m_name(name)
{
}




bool
window::
test_by_point(int  x, int  y) const noexcept
{
  return((x >= (m_position.x         )) &&
         (y >= (m_position.y         )) &&
         (x <  (m_position.x+m_width )) &&
         (y <  (m_position.y+m_height)));
}


void
window::
do_total_reform_if_necessary() noexcept
{
    if(m_whether_received_reform_request)
    {
      m_root->reform(point());

      int  w = m_root->get_width() ;
      int  h = m_root->get_height();

        if((w != m_content_image.get_width()) ||
           (h != m_content_image.get_height()))
        {
          m_content_image.resize(w,h);

          m_width  = w;
          m_height = h;
        }


      m_whether_received_reform_request = false;
      m_whether_need_to_total_redraw    =  true;
    }
}


void
window::
set_background_color(color  color) noexcept
{
  m_background_color = color;

  m_whether_need_to_total_redraw = true;
}


void
window::
set_root_widget(widget*  wg) noexcept
{
  m_update_counter = 0;

  m_root.reset(wg);

  m_current = nullptr;

    if(m_root)
    {
      m_root->set_window(this);

      m_whether_received_reform_request = true;

      do_total_reform_if_necessary();

      redraw_if_necessary();
    }
}


bool
window::
redraw_if_necessary() noexcept
{
    if(m_whether_need_to_total_redraw)
    {
      m_content_image.fill(m_background_color);

      m_root->redraw(m_content_image);

      m_whether_need_to_total_redraw = false;

      m_redraw_queue.clear();

      return true;
    }

  else
    if(m_redraw_queue.size())
    {
        for(auto  wg_ptr: m_redraw_queue)
        {
          wg_ptr->redraw(m_content_image);
        }


      m_redraw_queue.clear();

      return true;
    }


  return false;
}


void
window::
process_by_mouse_position(point&  pt) noexcept
{
    if(!m_current)
    {
      m_current = m_root->find_by_point(pt);

        if(m_current)
        {
          m_current->do_on_mouse_enter();
        }
    }

  else
    if(!m_current->test_by_point(pt))
    {
        if(g_input.test_mouse_left() ||
           g_input.test_mouse_right())
        {
          auto  cnt_pt = m_current->get_content_position();

          int    left = cnt_pt.x                                ;
          int   right = cnt_pt.x+m_current->get_content_width() ;
          int     top = cnt_pt.y                                ;
          int  bottom = cnt_pt.y+m_current->get_content_height();

               if(pt.x <   left){pt.x =  left  ;}
          else if(pt.x >= right){pt.x = right-1;}

               if(pt.y <     top){pt.y =    top  ;}
          else if(pt.y >= bottom){pt.y = bottom-1;}
        }

      else
        {
          m_current->do_on_mouse_leave();

          m_current = m_root->find_by_point(pt);

            if(m_current)
            {
              m_current->do_on_mouse_enter();
            }
        }
    }
}


void
window::
process_user_input(point  pt) noexcept
{
  pt -= m_position;

  process_by_mouse_position(pt);

    if(m_current)
    {
      m_current->do_on_mouse_act(pt-m_current->get_content_position());
    }
}


void
window::
print() const noexcept
{
  printf("window %s{...}\n",m_name.data());
}




}




