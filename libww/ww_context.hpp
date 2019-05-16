#ifndef ww_context_hpp_is_included
#define ww_context_hpp_is_included


#include"libww/ww_battle_section.hpp"




namespace ww{




class
context
{
  static constexpr int  m_screen_width  = 288;
  static constexpr int  m_screen_height = 288;

  uint32_t  m_spilling_text_counter=0;

  gbstd::process  m_process;

  gbstd::task_list     m_task_list;
  gbstd::task_control  m_task_control;

  gbstd::clock_control  m_clock_control;

  gbstd::clock_master  m_clock_master;

  battle_section  s_battle;

  party  m_left_party;
  party  m_right_party;


  static void  start_display_logo(gbstd::execution&  exec, context&  ctx) noexcept;
  static void   wait_display_logo(gbstd::execution&  exec, context&  ctx) noexcept;

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

  bool  has_active_spilling_text() const noexcept{return m_spilling_text_counter;}

  void  push_spilling_text(gbstd::color  color, std::u16string_view  sv, gbstd::point  center, uint32_t  time) noexcept;

  void  step(const gbstd::canvas&  cv) noexcept;

  static constexpr int  get_screen_width()  noexcept{return m_screen_width ;}
  static constexpr int  get_screen_height() noexcept{return m_screen_height;}

  static void  tick(gbstd::task_control  ctrl,                           context&  ctx) noexcept;
  static void  draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, context&  ctx) noexcept;

};


}




#endif




