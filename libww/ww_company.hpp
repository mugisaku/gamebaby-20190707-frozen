#ifndef ww_company_hpp_is_included
#define ww_company_hpp_is_included


#include"libww/ww_entry.hpp"
#include"libww/ww_bar.hpp"


namespace ww{




class
battle_side
{
  int  m_data;

public:
  static constexpr int   m_left_value = 0;
  static constexpr int  m_right_value = 0;

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

  operator int() const noexcept{return *m_data;}

  void   apply() noexcept{if(m_data && !m_flag){  ++*m_data;  m_flag =  true;}}
  void  cancel() noexcept{if(m_data &&  m_flag){  --*m_data;  m_flag = false;}}

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
time_add_amount_wrapper
{
  uint32_t*  m_value;

public:
  time_add_amount_wrapper(uint32_t&  v) noexcept: m_value(&v){}

  const uint32_t&  get(           ) const noexcept{return *m_value    ;}
  void             set(uint32_t  v) const noexcept{       *m_value = v;}

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




class
company
{
  tag  m_tag;

  struct flags{
    static constexpr int  motion_valid    = 1;
    static constexpr int  motion_paused   = 2;
    static constexpr int  motion_finished = 4;
  };


  status_value  m_status;

  entry*  m_original;
  entry   m_variable;

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

  gbstd::point  m_current_pos;

  bar  m_hp_bar;

  play_counter  m_play_counter;

public:
  company(int&  counter) noexcept;

  void  set_debug() const noexcept;

  void    set_hp_bar(bar&  hp_bar) noexcept{m_hp_bar = std::ref(hp_bar);}
  void  reset_hp_bar() noexcept;

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

  void  set_y_position(int  v) noexcept{m_current_pos.y  = v;}
  void  add_y_position(int  v) noexcept{m_current_pos.y += v;}

  void    set_white_flag() noexcept{m_white_flag =  true;}
  void  unset_white_flag() noexcept{m_white_flag = false;}

  blink_context&  get_blink_context() noexcept{return m_blink_context;}

  const int&  get_ap() const noexcept{return m_ap;}

  void  set_ap(int  v) noexcept{m_ap  = v;}
  void  add_ap(int  v) noexcept{m_ap += v;}

  entry&  get_variable() noexcept{return m_variable;}

  void        set_tag(battle_side  side, int  i, gbstd::color  color)       noexcept;
  const tag&  get_tag(                                              ) const noexcept{return m_tag;}

        gbstd::process&  get_process()       noexcept{return m_process;}
  const gbstd::process&  get_process() const noexcept{return m_process;}

  gbstd::point  get_current_position() const noexcept{return m_current_pos;}

  void  move_to_advance(int  n) noexcept{m_current_pos.x += m_tag.is_left()?  n:-n;}
  void  move_to_back(   int  n) noexcept{m_current_pos.x += m_tag.is_left()? -n: n;}

  void  reset(           ) noexcept;
  void  reset(entry&  org) noexcept;

  void  step(uint32_t  time) noexcept;

  void  render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept;

};


}




#endif




