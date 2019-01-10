#ifndef ww_hpp_is_included
#define ww_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include<list>
#include<vector>


namespace ww{


constexpr int  g_frame_w = gbstd::g_font_width *6;
constexpr int  g_frame_h = gbstd::g_font_height*2;


class force;
class row;
class battle_context;


enum class
side
{
  left,
  right,

};


class
battle_position
{
  int  m_value;

public:
  constexpr battle_position(int  v=0) noexcept: m_value(v){}

  constexpr operator int() const noexcept{return m_value;}

  constexpr bool  operator==(battle_position  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(battle_position  rhs) const noexcept{return m_value != rhs.m_value;}

};


constexpr battle_position   front_pos(0);
constexpr battle_position    back_pos(1);
constexpr battle_position  backup_pos(2);


class
window
{
protected:
  gbstd::point  m_pos;

  int  m_width =0;
  int  m_height=0;

public:
  

};


class
company
{
  std::string  m_name;

  int  m_hp=0;

  battle_position  m_pos=front_pos;

public:
  company() noexcept{}

  company(const char*  name, int  hp, battle_position  pos) noexcept:
  m_name(name), m_hp(hp), m_pos(pos){};

  const std::string&  get_name() const noexcept{return m_name;}

  int   get_hp() const noexcept{return m_hp;}
  void  set_hp(int  v) noexcept{m_hp  = v;}
  void  add_hp(int  v) noexcept{m_hp += v;}

  battle_position  get_position() const noexcept{return m_pos;}

  void  render(gbstd::color  force_color, const gbstd::canvas&  cv) const noexcept;

};


struct
force_initializer
{
  gbstd::color  m_color;

  std::vector<company*>  m_company_list;

  void  clear_list() noexcept{m_company_list.clear();}

  void  add(company*  com, int  n=1) noexcept
  {
      while(n--)
      {
        m_company_list.emplace_back(com++);
      }
  }

};


class
blink_context
{
  int  m_state=0;

public:
  void   enable() noexcept{m_state |= 1;}
  void  disable() noexcept{m_state  = 0;}

  bool  is_valid()   const noexcept{return m_state&1;}
  bool  is_visible() const noexcept{return m_state&2;}

  void  step() noexcept{if(is_visible()){m_state &= 1;} else{m_state |= 2;}}

};


struct
row
{
  force*  m_force=nullptr;

  company*  m_original;
  company   m_variable;

  int  m_ap=0;

  blink_context  m_blink_context;

  bool  m_white_flag=false;

  gbstd::process  m_process;

  gbstd::point  m_base_pos;
  gbstd::point  m_current_pos;

  bool  is_surviving() const noexcept{return m_original && m_variable.get_hp();}

  bool  is_left()  const noexcept;
  bool  is_right() const noexcept;

  const gbstd::process&  get_process() const noexcept{return m_process;}

  void  move_to_advance(int  n) noexcept{m_current_pos.x += -n;}
  void  move_to_back(   int  n) noexcept{m_current_pos.x -= -n;}

  void  ready() noexcept;

  void  reset(force&  force, company*  org) noexcept;

  void  step() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};


struct
force
{
  static constexpr int  m_number_of_rows = 3;

  row  m_rows[m_number_of_rows];

  uint32_t  m_total_hp=0;

  side  m_side=side::left;

  gbstd::color  m_color;

public:
  int  get_rows_by_position(battle_position  pos, row**  buf, int  n) noexcept;

  bool  can_continue_fight() const noexcept;

  void  reset(ww::side  side, const force_initializer&  init) noexcept;

  void  update_total_hp() noexcept;

  uint32_t  get_total_hp() const noexcept{return m_total_hp;}

  void  ready() noexcept;

  row*  get_actor_by_ap() noexcept;

  void  distribute_ap(int  v) noexcept;

  void  step() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

  row*  begin() noexcept{return std::begin(m_rows);}
  row*    end() noexcept{return   std::end(m_rows);}

};




struct
act_context
{
  battle_context*  m_battle_context;

  row*  m_row;

};


struct
attack_context
{
  battle_context*  m_battle_context;

  row*  m_actor;
  row*  m_target;

  void  reset(row&  actor, row&  target) noexcept
  {
    m_actor  = &actor;
    m_target = &target;
  }

};




class
battle_context
{
  force  m_left_force ;
  force  m_right_force;

  uint32_t  m_time=0;

  gbstd::text  m_text;

  gbstd::typewriter  m_typewriter;

  act_context  m_act_context;
  attack_context  m_attack_context;

  gbstd::process  m_process;

  uint32_t  m_display_flags=0;

  int  m_number_of_busy_players;

  bool  m_command_request;

  struct display_flags{
    static constexpr int  show_text = 1;
  };


  static void  fight(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  judge(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  judge_hit(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  wait_for_finish_to_attack(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  start_battle(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_message(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  wait_for_players(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_press_p_key(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  advance_time(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  open_command_window(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  show_message(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  hide_message(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  step_both_forces(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  ready_battle(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  wait_for_all(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  void  step_players() noexcept;
  void  update_number_of_busy_players() noexcept;

  void  pump_text() noexcept;
  void  pump_text_all() noexcept;

public:
  battle_context() noexcept{}

  template<typename...  Args>
  void  push_message(Args...  args) noexcept{m_typewriter.push(args...);}

  void  reset_text() noexcept;

  void    set_display_flag(int  flag) noexcept{m_display_flags |=  flag;}
  void  unset_display_flag(int  flag) noexcept{m_display_flags &= ~flag;}

  void  show_text() noexcept{  set_display_flag(display_flags::show_text);}
  void  hide_text() noexcept{unset_display_flag(display_flags::show_text);}

  void  reset(const force_initializer&  l,
              const force_initializer&  r) noexcept;

  void  ready() noexcept;

  void  step() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};




}




#endif




