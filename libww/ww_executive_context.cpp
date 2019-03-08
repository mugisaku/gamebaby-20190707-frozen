#include"libww/ww_executive_context.hpp"




namespace ww{




executive_context::
executive_context() noexcept
{
}




void
executive_context::
set_screen_size(int  w, int  h) noexcept
{
  m_screen_width  = w;
  m_screen_height = h;

  m_battle_context.set_field_size(w,h);
}


void
fn(gbstd::execution&  exec, executive_context*  ctx)
{report;}


void
executive_context::
startup() noexcept
{
  static entry  l_entries[] = {
    {"ひだりぐんA",1000,ww::front_pos},
    {"ひだりぐんB",1000,ww::front_pos},
    {"ひだりぐんC",1000,ww::front_pos},
  };

  static entry  r_entries[] = {
    {"みぎぐんA",1000,ww::front_pos},
    {"みぎぐんB",1000,ww::back_pos},
    {"みぎぐんC",1000,ww::front_pos},
  };


    for(auto&  e: l_entries)
    {
      m_battle_context.push_entry(e,battle_sides::left,gbstd::colors::blue);
    }


    for(auto&  e: r_entries)
    {
      m_battle_context.push_entry(e,battle_sides::right,gbstd::colors::red);
    }


  m_process.assign({
                    {initialize,this},
                    {loop_battle,this},
                   });


  m_task_list.push(80,m_process);

  push_major_task_list(m_task_list);
}


void
executive_context::
cleanup() noexcept
{
}




}




