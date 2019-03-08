#ifndef ww_company_hpp_is_included
#define ww_company_hpp_is_included


#include"libww/ww_entry.hpp"
#include"libww/ww_bar.hpp"
#include<cstring>


namespace ww{




class
battle_side
{
  int  m_data;

public:
  static constexpr int   m_left_value = 0;
  static constexpr int  m_right_value = 1;

  constexpr battle_side(int  v=0) noexcept: m_data(v){}

  constexpr bool  operator==(battle_side  rhs) const noexcept{return m_data == rhs.m_data;}
  constexpr bool  operator!=(battle_side  rhs) const noexcept{return m_data != rhs.m_data;}

  constexpr bool  is_left()  const noexcept{return m_data == m_left_value;}
  constexpr bool  is_right() const noexcept{return m_data == m_right_value;}

  constexpr battle_side  get_opposite() const noexcept{return battle_side(is_left()? m_right_value:m_left_value);}

};


namespace battle_sides{
constexpr battle_side   left = battle_side(battle_side::m_left_value);
constexpr battle_side  right = battle_side(battle_side::m_right_value);
}


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
play_counter
{
  int*  m_data=nullptr;

  bool  m_flag=false;

public:
  play_counter(int&  v) noexcept: m_data(&v){}
 ~play_counter(){cancel();}

  play_counter(const play_counter& ) noexcept=delete;
  play_counter(      play_counter&&) noexcept=delete;

  play_counter&  operator=(const play_counter& ) noexcept=delete;
  play_counter&  operator=(      play_counter&&) noexcept=delete;

  operator bool() const noexcept{return m_flag;}

  void   apply() noexcept{if(m_data && !m_flag){  ++*m_data;  m_flag =  true;}}
  void  cancel() noexcept{if(m_data &&  m_flag){  --*m_data;  m_flag = false;}}

};


class
tag: public battle_side
{
  int  m_index;

  gbstd::color  m_color;

public:
  tag() noexcept{}
  tag(battle_side  side, int  i, gbstd::color  color) noexcept:
  battle_side(side), m_index(i), m_color(color){}

  int  get_index() const noexcept{return m_index;}

  gbstd::color  get_color() const noexcept{return m_color;}

};


constexpr int  g_center_space_width  = 64;
constexpr int  g_cell_width  = 64;
constexpr int  g_cell_height = 48;
constexpr int  g_row_width  = g_cell_width*3;
constexpr int  g_row_height = g_cell_height;
constexpr int  g_column_width  = g_cell_width;
constexpr int  g_column_height = g_cell_height*6;




enum class
company_event_kind
{
  null,
  stop,
  move,
    set_blink,
  unset_blink,
    set_white,
  unset_white,

};


namespace company_events{
struct
stop
{
  uint32_t  m_delay;

  constexpr stop(uint32_t  delay=0) noexcept: m_delay(delay){}

  constexpr uint32_t  get_delay() const noexcept{return m_delay;}

};
struct   set_blink{};
struct unset_blink{};
struct   set_white{};
struct unset_white{};
struct
move
{
  uint32_t  m_count;
  uint32_t  m_delay;

  gbstd::fixed_t  m_x_increment;
  gbstd::fixed_t  m_y_increment;

  constexpr move(uint32_t  count=0, uint32_t  delay=0, gbstd::fixed_t  x_inc=0, gbstd::fixed_t  y_inc=0) noexcept:
  m_count(count), m_delay(delay), m_x_increment(x_inc), m_y_increment(y_inc){}

  constexpr const uint32_t&  get_delay() const noexcept{return m_delay;}
  constexpr const uint32_t&  get_count() const noexcept{return m_count;}

  constexpr const gbstd::fixed_t&  get_x_increment() const noexcept{return m_x_increment;}
  constexpr const gbstd::fixed_t&  get_y_increment() const noexcept{return m_y_increment;}

};
}


union
company_event_data
{
  company_events::stop  m_stop;
  company_events::move  m_move;

  company_event_data() noexcept{}
  constexpr company_event_data(company_events::stop  stp) noexcept: m_stop(stp){}
  constexpr company_event_data(company_events::move  mov) noexcept: m_move(mov){}

};


class
company_event
{
  company_event_kind  m_kind;
  company_event_data  m_data;

public:
  company_event() noexcept: m_kind(company_event_kind::null){}
  constexpr company_event(company_events::stop  stp) noexcept: m_kind(company_event_kind::stop), m_data(stp){}
  constexpr company_event(company_events::move  mov) noexcept: m_kind(company_event_kind::move), m_data(mov){}
  company_event(company_events::set_blink)   noexcept: m_kind(company_event_kind::set_blink  ){}
  company_event(company_events::unset_blink) noexcept: m_kind(company_event_kind::unset_blink){}
  company_event(company_events::set_white)   noexcept: m_kind(company_event_kind::set_white  ){}
  company_event(company_events::unset_white) noexcept: m_kind(company_event_kind::unset_white){}

  company_event&  operator=(const company_event&  rhs) noexcept
  {
    std::memcpy(this,&rhs,sizeof(*this));

    return *this;
  }

  const company_event_kind&  get_kind() const noexcept{return m_kind;}
  const company_event_data&  get_data() const noexcept{return m_data;}

  bool  is_null() const noexcept{return m_kind == company_event_kind::null;}
  bool  is_stop() const noexcept{return m_kind == company_event_kind::stop;}
  bool  is_move() const noexcept{return m_kind == company_event_kind::move;}
  bool  is_set_blink()   const noexcept{return m_kind == company_event_kind::set_blink;}
  bool  is_unset_blink() const noexcept{return m_kind == company_event_kind::unset_blink;}
  bool  is_set_white()   const noexcept{return m_kind == company_event_kind::set_white;}
  bool  is_unset_white() const noexcept{return m_kind == company_event_kind::unset_white;}

};


using real_point = gbstd::basic_point<gbstd::fixed_t>;


class
company
{
  tag  m_tag;

  struct flags{
    static constexpr int  motion_valid    = 1;
    static constexpr int  motion_paused   = 2;
    static constexpr int  motion_finished = 4;
  };


  gbstd::status_value<int>  m_status;

  entry*  m_original;
  entry   m_variable;

  int  m_ap=0;

  blink_context  m_blink_context;

  bool  m_white_flag=false;

  gbstd::process  m_process;

  std::vector<company_event>  m_event_queue;

  int  m_event_index=0;

  company_event  m_current_event;

  uint32_t  m_event_next_time;
  uint32_t  m_event_counter;


  real_point  m_offset;

  real_point  m_backup_point;
  real_point  m_back_point;
  real_point  m_front_point;
 
  real_point  m_name_point;
  real_point  m_hp_point;

  real_point  m_current_pos;

  bar  m_hp_bar;

  play_counter  m_play_counter;

  int  m_render_counter=0;

  static void  step_animation(gbstd::task_control&  ctrl, company*  c) noexcept;

  static void  render(const company&  c, const gbstd::canvas&  cv) noexcept;
  static void   drive(gbstd::task_control&  ctrl, const company*  c) noexcept;

public:
  company(int&  counter) noexcept;

  void  set_debug() const noexcept;

  void    set_hp_bar(bar&  hp_bar) noexcept{m_hp_bar = std::ref(hp_bar);}
  void  reset_hp_bar() noexcept;

  void  add_absolute_motion(real_point  dst, int  number_of_frames, uint32_t  ms) noexcept;
  void  add_relative_motion(real_point  off, int  number_of_frames, uint32_t  ms) noexcept;

  void  add_jump_motion() noexcept;
  void  add_attack_motion() noexcept;
  void  add_damage_motion() noexcept;

  void  clear_event_queue() noexcept;

  template<typename  T>
  void  push(T  t) noexcept{m_event_queue.emplace_back(t);}

  const real_point&  get_backup_point() const noexcept{return m_backup_point;}
  const real_point&  get_back_point()   const noexcept{return m_back_point;}
  const real_point&  get_front_point()  const noexcept{return m_front_point;}

  bool  is_surviving() const noexcept{return m_variable.get_hp();}

  void  set_y_position(int  v) noexcept{m_current_pos.y  = v;}
  void  add_y_position(int  v) noexcept{m_current_pos.y += v;}

  void    set_white_flag() noexcept{m_white_flag =  true;}
  void  unset_white_flag() noexcept{m_white_flag = false;}

  blink_context&  get_blink_context() noexcept{return m_blink_context;}

  const int&  get_ap() const noexcept{return m_ap;}

  void  set_ap(int  v) noexcept{m_ap  = v;}
  void  add_ap(int  v) noexcept{m_ap += v;}

        entry&  get_variable()       noexcept{return m_variable;}
  const entry&  get_variable() const noexcept{return m_variable;}

  void        set_tag(battle_side  side, int  i, gbstd::color  color)       noexcept;
  const tag&  get_tag(                                              ) const noexcept{return m_tag;}

  void               set_offset(real_point  off)       noexcept{       m_offset = off;}
  const real_point&  get_offset(               ) const noexcept{return m_offset      ;}

        gbstd::process&  get_process()       noexcept{return m_process;}
  const gbstd::process&  get_process() const noexcept{return m_process;}

  const real_point&  get_current_position() const noexcept{return m_current_pos;}

  void  move_to_advance(int  n) noexcept{m_current_pos.x += m_tag.is_left()?  n:-n;}
  void  move_to_back(   int  n) noexcept{m_current_pos.x += m_tag.is_left()? -n: n;}

  void  reset() noexcept;

  void  set_entry(entry&  org) noexcept;

  void  startup() noexcept;
  void  cleanup() noexcept;

};


template<typename  T, int  N>
class
filtering_table
{
  T*  m_buffer[N];

  int  m_number;

public:
  filtering_table() noexcept: m_number(0){}

  template<typename  IT>
  filtering_table(IT  begin_, IT  end_) noexcept{assign(begin_,end_);}

  template<typename  IT>
  filtering_table&  assign(IT  begin_, IT  end_) noexcept
  {
    m_number = 0;

      while(begin_ != end_)
      {
        push(&*begin_++);
      }


    return *this;
  }

  template<typename  TESTER>
  int  filter(TESTER  tester) noexcept
  {
    filtering_table  tmp(*this);

    m_number = 0;

      for(auto  ptr: tmp)
      {
          if(tester(*ptr))
          {
            push(ptr);
          }
      }


    return m_number;
  }

  T&  operator[](int  i) const noexcept{return *m_buffer[i];}

  T**  begin() noexcept{return &m_buffer[       0];}
  T**    end() noexcept{return &m_buffer[m_number];}

  void  push(T*  t) noexcept{m_buffer[m_number++] = t;}

  int  size() const noexcept{return m_number;}

};


}




#endif




