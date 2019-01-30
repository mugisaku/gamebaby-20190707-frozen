#include"libww/ww.hpp"




namespace ww{




battle_context::
battle_context(time_standard  tmstd) noexcept:
m_time_standard(tmstd),
m_left_force( battle_side::left ,m_time),
m_right_force(battle_side::right,m_time)
{
  m_text.resize(22,6);

  m_typewriter.assign(m_text);


  m_act_context.m_battle_context    = this;
  m_attack_context.m_battle_context = this;

m_left_force.begin()->set_debug();
}




void
battle_context::
set_field_size(int  w, int  h) noexcept
{
  m_field_width  = w;
  m_field_height = h;
}


void
battle_context::
reset(const force_initializer&  l,
      const force_initializer&  r) noexcept
{
   m_left_force.reset(l);
  m_right_force.reset(r);

  reset();
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
reset() noexcept
{
  m_process.assign({{start_battle,this}});

   m_left_force.reset();
  m_right_force.reset();

  m_text.fill(gbstd::character());

  m_typewriter.set_cursor_position({0,0});

  m_time            = 0;
  m_time_add_amount = 0;

  m_command_request = true;
}


void
battle_context::
step_notifiers() noexcept
{
  auto  it = m_notifiers.begin();

    while(it != m_notifiers.end())
    {
      auto&  n = *it;

        if(!n.is_halted())
        {
          n.step();

          ++it;
        }

      else
        {
          it = m_notifiers.erase(it);
        }
    }
}


void
battle_context::
step() noexcept
{
  m_time += m_time_add_amount;

  step_notifiers();

  m_process.step();
}


void
battle_context::
render(const gbstd::canvas&  cv) const noexcept
{
  gbstd::point  offset(m_field_width/2,0);

   m_left_force.render(offset,cv);
  m_right_force.render(offset,cv);

    if(m_display_flags&display_flags::show_text)
    {
      static constexpr gbstd::point  m_console_pos = gbstd::point(80,160);

      cv.draw_typewriter(m_typewriter,m_console_pos);
    }


    for(auto&  n: m_notifiers)
    {
      n.render(offset,cv);
    }
}




}




