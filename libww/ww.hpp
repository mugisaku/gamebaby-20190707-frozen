#ifndef ww_hpp_is_included
#define ww_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include<list>
#include<vector>


namespace ww{




class force;
class row;
class battle_context;


enum class
battle_side
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

  bool  is_front_position()  const noexcept{return m_pos == front_pos;}
  bool  is_back_position()   const noexcept{return m_pos == back_pos;}
  bool  is_backup_position() const noexcept{return m_pos == backup_pos;}

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


class
counter
{
  int*  m_data=0;

public:
  constexpr counter() noexcept: m_data(nullptr){}
  constexpr counter(int&  v) noexcept: m_data(&v){}

  operator int() const noexcept{return *m_data;}

  counter&  operator++() noexcept{  ++*m_data;  return *this;}
  counter&  operator--() noexcept{  --*m_data;  return *this;}

  counter  operator++(int) noexcept{  auto  t = *this;  ++*m_data;  return t;}
  counter  operator--(int) noexcept{  auto  t = *this;  --*m_data;  return t;}

};


class
time_wrapper
{
  const uint32_t*  m_target=nullptr;

public:
  time_wrapper() noexcept{}
  time_wrapper(const uint32_t&  tm) noexcept: m_target(&tm){}

  const uint32_t&  get() const noexcept{return *m_target;}

};


class
time_standard
{
  uint32_t  m_value;

public:
  constexpr time_standard(uint32_t  v=0) noexcept: m_value(v){}

  constexpr operator uint32_t() const noexcept{return m_value;}

  constexpr uint32_t  get() const noexcept{return m_value;}

};


class
status_value
{
  uint32_t  m_data;

public:
  status_value(uint32_t  flag=0) noexcept: m_data(flag){}

  void  clear() noexcept{m_data = 0;}

  void  reverse(uint32_t  flag) noexcept
  {
      if(test(flag)){unset(flag);}
    else            {  set(flag);}
  }

  void    set(uint32_t  flag) noexcept{m_data |=  flag;}
  void  unset(uint32_t  flag) noexcept{m_data &= ~flag;}
  bool   test(uint32_t  flag) const noexcept{return m_data&flag;}

};


class
bar
{
  gbstd::point  m_pos;

  int  m_thickness;

  uint32_t  m_max;
  uint32_t  m_current;

  gbstd::color  m_color;

public:
  bar(gbstd::point  pos, int  thickness, uint32_t  max, uint32_t  cur=0) noexcept:
  m_pos(pos), m_thickness(thickness), m_max(max), m_current(cur){}

  const gbstd::point&  get_position() const noexcept{return m_pos;}

  const int&  get_thickness() const noexcept{return m_thickness;}

  const uint32_t&  get_current() const noexcept{return m_current;}
  const uint32_t&  get_max()     const noexcept{return m_max;}

  void  set_current(uint32_t  v) noexcept{m_current = v;}
  void  set_max(    uint32_t  v) noexcept{m_max     = v;}

  const gbstd::color&  get_color(                   ) const noexcept{return m_color        ;}
  void                 set_color(gbstd::color  color)       noexcept{       m_color = color;}

};


class
row
{
  struct flags{
    static constexpr int  motion_valid    = 1;
    static constexpr int  motion_paused   = 2;
    static constexpr int  motion_finished = 4;
  };


  status_value  m_status;

  time_wrapper  m_time;

  counter  m_moving_row_counter;

  int  m_index;

  force*  m_force=nullptr;

  company*  m_original;
  company   m_variable;

  int  m_ap=0;

  blink_context  m_blink_context;

  bool  m_white_flag=false;

  gbstd::process  m_process;

  struct motion_frame{
    gbstd::point  m_pos;

    uint32_t  m_delay;

    constexpr motion_frame(gbstd::point  pos, uint32_t  delay) noexcept:
    m_pos(pos), m_delay(delay){}

  };

  std::vector<motion_frame>  m_motion_frames;

  uint32_t  m_motion_timer=0;

  int  m_motion_frame_index=0;

  gbstd::point  m_backup_point;
  gbstd::point  m_back_point;
  gbstd::point  m_front_point;
 
  gbstd::point  m_name_point;
  gbstd::point  m_hp_point;
  gbstd::point  m_bar_point;

  gbstd::point  m_current_pos;

public:
  void  set_debug() const noexcept;

  void  clear_motion() noexcept{m_motion_frames.clear();}
  void  rewind_motion() noexcept;
  void  add_motion(gbstd::point  dst, int  number_of_frames, uint32_t  ms) noexcept;
  void    pause_motion() noexcept;
  void  unpause_motion() noexcept;

  bool  is_motion_finished() const noexcept{return m_status.test(flags::motion_finished);}

  const gbstd::point&  get_backup_point() const noexcept{return m_backup_point;}
  const gbstd::point&  get_back_point()   const noexcept{return m_back_point;}
  const gbstd::point&  get_front_point()  const noexcept{return m_front_point;}

  bool  is_surviving() const noexcept{return m_original && m_variable.get_hp();}

  bool  is_left()  const noexcept;
  bool  is_right() const noexcept;

  void  set_y_position(int  v) noexcept{m_current_pos.y  = v;}
  void  add_y_position(int  v) noexcept{m_current_pos.y += v;}

  void    set_white_flag() noexcept{m_white_flag =  true;}
  void  unset_white_flag() noexcept{m_white_flag = false;}

  blink_context&  get_blink_context() noexcept{return m_blink_context;}

  const int&  get_ap() const noexcept{return m_ap;}

  void  set_ap(int  v) noexcept{m_ap  = v;}
  void  add_ap(int  v) noexcept{m_ap += v;}

  company&  get_variable() noexcept{return m_variable;}

  void  set_time(time_wrapper  tm) noexcept{m_time = tm;}

  void  set_force(force&  f, counter  mov_row_counter, int  i) noexcept;

  force*  get_force() const noexcept{return m_force;}

        gbstd::process&  get_process()       noexcept{return m_process;}
  const gbstd::process&  get_process() const noexcept{return m_process;}

  gbstd::point  get_current_position() const noexcept{return m_current_pos;}

  void  move_to_advance(int  n) noexcept{m_current_pos.x += is_left()?  n:-n;}
  void  move_to_back(   int  n) noexcept{m_current_pos.x += is_left()? -n: n;}

  void  reset(                            ) noexcept;
  void  reset(force&  force, company*  org) noexcept;

  void  step() noexcept;

  void  render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept;

  struct callbacks{
    static void  start_motion(gbstd::execution&  exec, ww::row*  row) noexcept;
    static void  wait_for_motion(gbstd::execution&  exec, ww::row*  row) noexcept;

    static void    pause_motion(gbstd::execution&  exec, ww::row*  row) noexcept;
    static void  unpause_motion(gbstd::execution&  exec, ww::row*  row) noexcept;

  };

};


constexpr int  g_center_space_width  = 64;
constexpr int  g_cell_width  = 64;
constexpr int  g_cell_height = 48;
constexpr int  g_row_width  = g_cell_width*3;
constexpr int  g_row_height = g_cell_height;
constexpr int  g_column_width  = g_cell_width;
constexpr int  g_column_height = g_cell_height*6;


class
force
{
  static constexpr int  m_number_of_rows = 3;

  row  m_rows[m_number_of_rows];

  uint32_t  m_total_hp=0;

  battle_side  m_side;

  time_wrapper  m_time;

  gbstd::color  m_color;

  int  m_moving_row_count=0;

public:
  force(battle_side  side, time_wrapper  tm) noexcept;

  bool  operator==(battle_side  side) const noexcept{return m_side == side;}
  bool  operator!=(battle_side  side) const noexcept{return m_side != side;}

  const int&  get_moving_row_count() const noexcept{return m_moving_row_count;}

  const gbstd::color&  get_color() const noexcept{return m_color;}

  int  get_rows_by_position(battle_position  pos, row**  buf, int  n) noexcept;

  bool  can_continue_fight() const noexcept;

  void  reset(                              ) noexcept;
  void  reset(const force_initializer&  init) noexcept;

  void  update_total_hp() noexcept;

  uint32_t  get_total_hp() const noexcept{return m_total_hp;}

  row*  get_actor_by_ap() noexcept;

  void  distribute_ap(int  v) noexcept;

  void  step() noexcept;

  void  render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept;

  row*  begin() noexcept{return std::begin(m_rows);}
  row*    end() noexcept{return   std::end(m_rows);}

  struct callbacks{
    static void  start_motion(gbstd::execution&  exec, ww::force*  force) noexcept;
    static void  wait_for_motion(gbstd::execution&  exec, ww::force*  force) noexcept;
  };

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
notifier
{
  uint32_t  m_next_time;

  double  m_y_vector;

  double  m_x_pos;
  double  m_y_pos;

  gbstd::point  m_start_pos;

  std::string  m_text;

public:
  notifier(gbstd::point  start_pos, const char*  s) noexcept;

  void  step() noexcept;

  bool  is_halted() const noexcept{return m_text.empty();}

  void  render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept;

};


class
battle_context
{
  int  m_field_width =0;
  int  m_field_height=0;

  force  m_left_force ;
  force  m_right_force;

  uint32_t  m_time;
  uint32_t  m_time_add_amount;

  time_standard  m_time_standard;

  gbstd::text  m_text;

  gbstd::typewriter  m_typewriter;

  act_context  m_act_context;
  attack_context  m_attack_context;

  gbstd::process  m_process;

  uint32_t  m_display_flags=0;

  int  m_number_of_busy_players;

  bool  m_command_request;

  std::list<notifier>  m_notifiers;

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

  static void     start_motion(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void  wait_for_motion(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  static void  start_time(gbstd::execution&  exec, battle_context*  ctx) noexcept;
  static void   stop_time(gbstd::execution&  exec, battle_context*  ctx) noexcept;

  void  step_players() noexcept;
  void  update_number_of_busy_players() noexcept;

  void  step_notifiers() noexcept;

  void  pump_text() noexcept;
  void  pump_text_all() noexcept;

public:
  battle_context(time_standard  tmstd) noexcept;

  template<typename...  Args>
  void  push_message(Args...  args) noexcept{m_typewriter.push(args...);}

  void  set_field_size(int  w, int  h) noexcept;

  int  get_field_width()  const noexcept{return m_field_width ;}
  int  get_field_height() const noexcept{return m_field_height;}

  void  reset_text() noexcept;

  void    set_display_flag(int  flag) noexcept{m_display_flags |=  flag;}
  void  unset_display_flag(int  flag) noexcept{m_display_flags &= ~flag;}

  void  show_text() noexcept{  set_display_flag(display_flags::show_text);}
  void  hide_text() noexcept{unset_display_flag(display_flags::show_text);}

  void  reset(const force_initializer&  l,
              const force_initializer&  r) noexcept;

  void  reset() noexcept;

  void  step() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};


class
exective_context
{
  battle_context  m_battle_context;

};


}




#endif




