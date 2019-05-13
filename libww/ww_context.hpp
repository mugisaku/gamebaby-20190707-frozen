#ifndef ww_context_hpp_is_included
#define ww_context_hpp_is_included


#include"libww/ww_battle_section.hpp"




namespace ww{




class
context
{
  struct spilling_text_entry{
    gbstd::task_control  ctrl;
  };

  static constexpr int  m_screen_width  = ww::battles::g_center_space_width+(ww::battles::g_row_width*2);
  static constexpr int  m_screen_height = (ww::battles::g_column_height);


  gbstd::process  m_process;



  struct{
    gbstd::task_list  root;

    gbstd::task_control  battle_ctrl;

  } s_tasks;

  struct{
    gbstd::clock_control  system;

  } s_clock_controls;


  gbstd::clock_master  m_clock_master;

  battle_section  s_battle;

  party  m_left_party;
  party  m_right_party;


  static void  start(       gbstd::execution&  exec, context&  ctx) noexcept;
  static void  start_battle(gbstd::execution&  exec, context&  ctx) noexcept;
  static void  loop_battle( gbstd::execution&  exec, context&  ctx) noexcept;


  static void  initialize_battle(gbstd::execution&  exec, context&  ctx) noexcept;
  static void         run_battle(gbstd::execution&  exec, context&  ctx) noexcept;
  static void    finalize_battle(gbstd::execution&  exec, context&  ctx) noexcept;

  static void  wait_until_end_movie(gbstd::execution&  exec, context&  ctx) noexcept;

  battle_result  judge_battle() const noexcept;

  void  fight(gbstd::execution&  exec) noexcept;
  void  distribute_ap(int  v) noexcept;

  bool  judge_hit(const battles::character&  actor, const battles::character&  target) const noexcept;
  int   calculate_damage(const battles::character&  actor, const battles::character&  target) const noexcept;

  battles::character*  get_battle_character_by_ap() noexcept;

public:
  context() noexcept;

  void  step(const gbstd::canvas&  cv) noexcept;

  static constexpr int  get_screen_width()  noexcept{return m_screen_width ;}
  static constexpr int  get_screen_height() noexcept{return m_screen_height;}

};


}




#endif




