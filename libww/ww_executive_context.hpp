#ifndef ww_executive_context_hpp_is_included
#define ww_executive_context_hpp_is_included


#include"libww/ww_battle_context.hpp"
#include"libww/ww_notifier.hpp"


namespace ww{




class
executive_context
{
  int  m_screen_width =0;
  int  m_screen_height=0;

  gbstd::process      m_process;
  gbstd::task_list  m_task_list;

  battle_context  m_battle_context;

  gbstd::image  m_misc_image;

  static void  initialize(gbstd::execution&  exec, executive_context*  ctx) noexcept;

  static void  loop_battle(gbstd::execution&  exec, executive_context*  ctx) noexcept;

  static void  enter_battle(gbstd::execution&  exec, executive_context*  ctx) noexcept;
  static void   exit_battle(gbstd::execution&  exec, executive_context*  ctx) noexcept;

public:
  executive_context() noexcept;

  void  set_screen_size(int  w, int  h) noexcept;

  int  get_screen_width()  const noexcept{return m_screen_width ;}
  int  get_screen_height() const noexcept{return m_screen_height;}

  void  startup() noexcept;
  void  cleanup() noexcept;

};


}




#endif




