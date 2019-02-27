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
    {"ひだりぐんA",3000,ww::front_pos},
    {"ひだりぐんB",3000,ww::front_pos},
    {"ひだりぐんC",3000,ww::front_pos},
  };

  static entry  r_entries[] = {
    {"みぎぐんA",3000,ww::front_pos},
    {"みぎぐんB",3000,ww::back_pos},
    {"みぎぐんC",3000,ww::front_pos},
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


  gbstd::g_major_task_list.push(80,m_process);
}


void
executive_context::
cleanup() noexcept
{
}




}




