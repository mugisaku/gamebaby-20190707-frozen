#include"libww/ww.hpp"




namespace ww{




void
battle_context::
reset(const force_initializer&  l,
      const force_initializer&  r) noexcept
{
   m_left_force.reset(side::left ,l);
  m_right_force.reset(side::right,r);

  m_text.resize(22,6);

  m_typewriter.assign(m_text);


  m_act_context.m_battle_context    = this;
  m_attack_context.m_battle_context = this;

  m_process.assign(120,{{start_battle,this}});
//  m_process.set_verbose_flag();

  ready();
}


void
battle_context::
reset_text() noexcept
{
  m_text.fill(gbstd::character());

  m_typewriter.set_cursor_position({0,0});
}


void
battle_context::
ready() noexcept
{
   m_left_force.ready();
  m_right_force.ready();

  m_text.fill(gbstd::character());

  m_typewriter.set_cursor_position({0,0});

  m_command_request = true;
}


void
battle_context::
step() noexcept
{
  m_process.step();
}


void
battle_context::
render(const gbstd::canvas&  cv) const noexcept
{
   m_left_force.render({cv,                             0,0,g_frame_w*3,g_frame_h*8});
  m_right_force.render({cv,cv.get_width()-1-(g_frame_w*3),0,g_frame_w*3,g_frame_h*8});

    if(m_display_flags&display_flags::show_text)
    {
      static constexpr gbstd::point  m_console_pos = gbstd::point(80,160);

      cv.draw_typewriter(m_typewriter,m_console_pos);
    }
}




}




