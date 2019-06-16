#include"libww/ww_spilling_text.hpp"




namespace ww{




spilling_text::
spilling_text() noexcept:
m_task("spilling_text",this)
{
}




spilling_text&
spilling_text::
reset(gbstd::point  bottom_pos, int  offset) noexcept
{
  m_bottom_pos =   bottom_pos;
  m_pos        = m_bottom_pos;

  m_pos.y += offset;

  m_y_vector = -8;

  return *this;
}


void
spilling_text::
tick(spilling_text&  spltxt) noexcept
{
/*
    if(ctrl.test_timer_flag())
    {
        if(!ctrl.test_timer())
        {
          spltxt.m_text.clear();

          ctrl.set_remove_flag();
        }
    }

  else
*/
    {
      spltxt.m_pos.y += spltxt.m_y_vector     ;
                        spltxt.m_y_vector += 2;


        if(spltxt.m_pos.y >= spltxt.m_bottom_pos.y+16)
        {
          spltxt.m_y_vector /= 4;

            if(spltxt.m_y_vector > 1)
            {
              spltxt.m_y_vector = -spltxt.m_y_vector;
            }

          else
            {
              spltxt.m_task.sleep(spltxt.m_time);
            }
        }
    }
}


void
spilling_text::
draw(const gbstd::canvas&  cv, spilling_text&  spltxt) noexcept
{
  cv.draw_string_safely(spltxt.m_color,spltxt.m_text.data(),spltxt.m_pos.x,
                                                            spltxt.m_pos.y);
}




namespace{
spilling_text*
g_dead_top;
}


spilling_text*
spilling_text::
produce(uint32_t&  counter) noexcept
{
  spilling_text*  ptr;

    if(g_dead_top)
    {
      ptr = g_dead_top              ;
            g_dead_top = ptr->m_next;
    }

  else
    {
      ptr = new spilling_text;
    }


  ptr->m_counter = &counter;

  ++counter;

  return ptr;
}


void
spilling_text::
collect(spilling_text*  ptr) noexcept
{
    if(ptr)
    {
      --*ptr->m_counter;

      ptr->m_next = g_dead_top      ;
                    g_dead_top = ptr;
    }
}




}




