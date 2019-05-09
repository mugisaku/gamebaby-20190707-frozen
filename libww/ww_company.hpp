#ifndef ww_company_hpp_is_included
#define ww_company_hpp_is_included


#include"libww/ww_entry.hpp"
#include"libww/ww_bar.hpp"
#include<cstring>


namespace ww{




class context;


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


enum class
operation_kind
{
  null,
  level_up,
  level_down,
  hp_up,
  hp_down,
  mp_up,
  mp_down,
  hp_max_up,
  hp_max_down,
  mp_max_up,
  mp_max_down,

};


class
instruction
{
  operation_kind  m_kind;

  int  m_value;

public:
  constexpr instruction(operation_kind  k=operation_kind::null, int  v=0) noexcept:
  m_kind(k), m_value(v){}

  constexpr operation_kind  get_kind() const noexcept{return m_kind;}
  constexpr int  get_value() const noexcept{return m_value;}

};




}




#endif




