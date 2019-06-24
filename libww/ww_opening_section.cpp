#include"libww/ww_opening_section.hpp"
#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




namespace{
const scene
g_scenes[] =
{
  {{0,80},"text00"},
  {{0,80},"text01"},
  {{0,80},"text02"},
  {{0,80},"text03"},
  {{0,80},"text04"},
  {{0,80},"text05"},
  {{0,80},"text06"},
  {{0,80},"text07"},
  {{0,80},"text08"},
};
}




opening_section::
opening_section(system&  sys, gbstd::execution&  exec) noexcept:
m_system(sys),
m_scene(std::begin(g_scenes)),
m_scene_end(std::end(g_scenes))
{
  exec.get_equipped_clock().clear().set_permil(1000);

  m_task.set_data(this)
    .live()
    .show()
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

  advance();
}


opening_section::
~opening_section()
{
}




bool
opening_section::
advance() noexcept
{
    if(m_scene < m_scene_end)
    {
      auto&  scn = *m_scene++;

      auto  sv = m_system.get_text_section("opening")->get(scn.m_text_id);

      m_delay = 120*sv.size();

      m_text.clear().
        push(sv);

      m_text_base_pos_y = scn.m_text_position_base.y;

      return true;
    }


  return false;
}


void
opening_section::
tick(opening_section&  op) noexcept
{
  op.m_text.expose_all_characters_of_current_line();
}


void
opening_section::
draw(const gbstd::canvas&  cv, opening_section&  op) noexcept
{
  auto  it     = op.m_text.begin();
  auto  it_end = op.m_text.end();

  int  y = op.m_text_base_pos_y;

  constexpr int  x_center = context::get_screen_width()/2;

    while(it != it_end)
    {
      auto&  ln = *it++;

      auto  sv = ln.get_view();

      int  w = gbstd::g_font_width*sv.size();

      cv.draw_string(gbstd::colors::white,sv,x_center-(w/2),y);

      y += gbstd::g_font_height;
    }
}


void
opening_section::
run(gbstd::execution&  exec, opening_section&  op) noexcept
{
    if(gbstd::g_input.test_p())
    {
      op.m_task.discard();

      exec.pop_current_context(2);

      return;
    }


    if(op.m_text.is_displaying_all_characters())
    {
      auto&  cl = exec.get_equipped_clock();

        if(!cl)
        {
          cl.reset().turnon();
        }

      else
        if(cl.get_time() >= op.m_delay)
        {
            if(!op.advance())
            {
              op.m_task.discard();

              exec.pop_current_context(1);
            }


          cl.turnoff();
        }
    }
}




}




