#include"libww/ww_spilling_text.hpp"
#include"libww/ww_system.hpp"




namespace ww{




spilling_text::
spilling_text(system&  sys) noexcept:
m_system(sys),
m_task(this)
{
}




gbstd::task&
spilling_text::
initialize_task(gbstd::clock_watch  w) noexcept
{
  m_task.set_name("spilling_text")
    .live()
    .show()
    .set_interval(20)
    .set_draw(draw)
    .set_tick(tick)
    .set_finish(finish)
    .set_clock_watch(w);

  return m_task;
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
          spltxt.m_task.sleep(spltxt.m_time)
            .die_when_getup()
            .discard_when_die();
        }
    }
}


void
spilling_text::
draw(const gbstd::canvas&  cv, spilling_text&  spltxt) noexcept
{
  cv.draw_string_safely(spltxt.m_color,spltxt.m_text.data(),spltxt.m_pos.x,spltxt.m_pos.y);
}


void
spilling_text::
finish(spilling_text&  spltxt) noexcept
{
  spltxt.m_system.collect_spilling_text(spltxt);
}




}




