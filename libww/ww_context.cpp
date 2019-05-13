#include"libww/ww_context.hpp"




namespace ww{




context::
context() noexcept
{
  auto  sysclock = m_clock_master.add("system",1000);

  sysclock.start();

  s_clock_controls.system = sysclock;
  s_battle.m_clock_watch  = sysclock;

  s_tasks.battle_ctrl = s_tasks.root.push(s_battle.m_task_list);


  m_process.push({{"context::start",start,*this,gbstd::interruptions::on}},"");
}




void
context::
step(const gbstd::canvas&  cv) noexcept
{
  m_clock_master.step();

  s_tasks.root.process(cv);

  m_process.step();
}




}




