#include"libww/ww_context.hpp"




namespace ww{




context::
context() noexcept
{
  s_clock_controls.system = m_clock_master.add("system",1000);

  s_battle.m_clock_watch = s_clock_controls.system;

  s_draw_tasks.battle_ctrl = s_draw_tasks.root.push(                           s_battle.m_draw_task_list);
  s_tick_tasks.battle_ctrl = s_tick_tasks.root.push(s_clock_controls.system,20,s_battle.m_tick_task_list);


  m_process.push({{"context::start",start,*this,gbstd::interruptions::on}},"");
}




void
context::
step() noexcept
{
  m_clock_master.step();

  s_tick_tasks.root.process();

  m_process.step();
}


void
context::
render(const gbstd::canvas&  cv) noexcept
{
  s_draw_tasks.root.process(cv);
}




}




