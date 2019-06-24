#include"libww/ww_front_section.hpp"
#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




namespace{


const uint8_t
g_png_data[] =
{
#include"title.txt"
};


gbstd::image
g_image(g_png_data);
}


front_section::
front_section(system&  sys, gbstd::execution&  exec) noexcept:
m_system(sys),
m_phase(0)
{
  exec.get_equipped_clock().clear().set_permil(1000);

  m_task.set_data(this)
    .set_interval(20)
    .set_draw(draw)
    .set_tick(tick)
    .set_clock_watch(*exec.get_clock_by_id("standard"))
    .update_next_time()
  ;

  exec.add_task(m_task);

  exec.push_frame({
    {"*",run,*this},
  });
}


front_section::
~front_section()
{
}




void
front_section::
tick(front_section&  fr) noexcept
{
}


void
front_section::
draw(const gbstd::canvas&  cv, front_section&  fr) noexcept
{
  gbstd::canvas  src_cv0(g_image,{0,0,48*5,48});
  gbstd::canvas  src_cv1(g_image,{0,48,48,48});
  gbstd::canvas  src_cv2(g_image,{48,48,48*3,24});
  gbstd::canvas  src_cv3(g_image,{48,48+24,24,24});

  cv.draw_canvas(src_cv0,20,40);
  cv.draw_canvas(src_cv1,0,20);
  cv.draw_canvas(src_cv2,70,88);
  cv.draw_canvas(src_cv3,40,10);
}


void
front_section::
run(gbstd::execution&  exec, front_section&  fr) noexcept
{
    if(exec.is_jumped())
    {
      auto&  cl = exec.get_equipped_clock();

        if(fr.m_phase == 0)
        {
          fr.m_task.live().show();

          cl.clear().turnon();

          fr.m_phase = 1;
        }

      else
        if(cl.get_time() >= 8000)
        {
          exec.unset_jump();

          fr.m_phase = 0;

          fr.m_task.die();
        }
    }

  else
    {
        switch(fr.m_phase)
        {
      case(0):
          exec.push_current_context();

          new(&fr.m_opening_section) opening_section(fr.m_system,exec);
          break;
      case(1):
          break;
        }
    }
}




}




