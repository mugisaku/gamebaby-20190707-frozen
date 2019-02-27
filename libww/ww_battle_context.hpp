#ifndef ww_battle_context_hpp_is_included
#define ww_battle_context_hpp_is_included


#include"libgbstd/stage.hpp"
#include"libww/ww_company.hpp"
#include"libww/ww_notifier.hpp"
#include<list>
#include<vector>
#include<functional>


namespace ww{




class battle_context;


struct
act_context
{
  battle_context*  m_battle_context;

  company*  m_company;

};


struct
attack_context
{
  battle_context*  m_battle_context;

  company*  m_actor;
  company*  m_target;

  void  reset(company&  actor, company&  target) noexcept
  {
    m_actor  = &actor;
    m_target = &target;
  }

};




class
battle_context
{
  enum class result{
    continuing,
    draw,
    left_won,
    right_won,

  };

  result  m_result;

  int  m_field_width =0;
  int  m_field_height=0;

  std::list<company>  m_company_list;

  int  m_number_of_left_companies =0;
  int  m_number_of_right_companies=0;

  int  m_number_of_total_companies=0;

  int  m_number_of_playing_companies=0;

  gbstd::text  m_text;

  gbstd::typewriter  m_typewriter;

  act_context  m_act_context;
  attack_context  m_attack_context;

  gbstd::process  m_process;

  uint32_t  m_display_flags=0;

  bool  m_command_request;

  std::list<notifier>  m_notifiers;

  struct display_flags{
    static constexpr int  show_text = 1;
  };


  static void  fight(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  branch_by_judge(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  judge_hit(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_finish_to_attack(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_message(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  wait_for_players(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_press_p_key(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  open_command_window(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  show_message(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  hide_message(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  ready_battle(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_all(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_motion(gbstd::execution&  exec, battle_context*  ctx) noexcept;


  result  judge() const noexcept;

  void  update_number_of_busy_players() noexcept;

  void  step_notifiers() noexcept;

  void  pump_text() noexcept;
  void  pump_text_all() noexcept;

public:
  battle_context() noexcept;

  template<typename...  Args>
  void  push_message(Args...  args) noexcept{m_typewriter.push(args...);}

  bool  is_continuing() const noexcept{return m_result == result::continuing;}
  bool  is_draw()       const noexcept{return m_result == result::draw;}
  bool  is_left_won()   const noexcept{return m_result == result::left_won;}
  bool  is_right_won()  const noexcept{return m_result == result::right_won;}

  void  set_field_size(int  w, int  h) noexcept;

  int  get_field_width()  const noexcept{return m_field_width ;}
  int  get_field_height() const noexcept{return m_field_height;}

  company*  get_company_by_ap() noexcept;
  int       get_companies_by_side(battle_side  side, company**  begin, company**  end) noexcept;

  void  push_entry(entry&  ent, battle_side  side, gbstd::color  color) noexcept;

  void  distribute_ap(int  v) noexcept;

  void  reset_text() noexcept;

  void    set_display_flag(int  flag) noexcept{m_display_flags |=  flag;}
  void  unset_display_flag(int  flag) noexcept{m_display_flags &= ~flag;}

  void  show_text() noexcept{  set_display_flag(display_flags::show_text);}
  void  hide_text() noexcept{unset_display_flag(display_flags::show_text);}

  void  startup() noexcept;
  void  cleanup() noexcept;

  static void  drive(uint32_t&  delay, battle_context*  ctx) noexcept;
  static void  render(const battle_context&  ctx, const gbstd::canvas&  cv) noexcept;

};


}




#endif




