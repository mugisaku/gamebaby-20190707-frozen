#include"libww/ww_context.hpp"




namespace ww{




context::
context() noexcept
{
  auto  sysclock = m_clock_master.add("system",1000);

  sysclock.start();

  m_clock_control = sysclock;
  s_battle.m_clock_watch  = sysclock;

  s_battle.m_task_control = m_task_list.push(s_battle.m_task_list);

  m_task_list.push(*this)
             .set_draw<context>()
             .set_tick<context>(sysclock,20);


  m_process.push({{"context::start",start,*this,gbstd::interruptions::on}},"");
}




void
context::
push_spilling_text(gbstd::color  color, std::u16string_view  sv, gbstd::point  center, uint32_t  time) noexcept
{
  auto  ptr = spilling_text::produce(m_spilling_text_counter);

  auto  text_width = static_cast<int>(gbstd::g_font_width*sv.size());

  ptr->set_text(sv)
      .set_color(color)
      .set_time(time)
      .reset({center.x-(text_width/2),center.y-8},-32);

  m_task_list.push(*ptr).set_draw<spilling_text>()
                        .set_tick<spilling_text>(m_clock_control,20)
                        .set_collect<spilling_text>();
}


void
context::
tick(gbstd::task_control  ctrl, context&  ctx) noexcept
{
}


void
context::
draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, context&  ctx) noexcept
{
}


void
context::
step(const gbstd::canvas*  cv) noexcept
{
    if(cv)
    {
      cv->fill(m_background_color);
    }


  m_clock_master.step();

  m_task_list.process(cv,true);

  m_process.step();
}




}




