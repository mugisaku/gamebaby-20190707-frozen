#ifndef ww_field_hpp_is_included
#define ww_field_hpp_is_included


#include"libww/ww_bar.hpp"




namespace ww{
namespace battles{




class
side
{
  int  m_data;

public:
  static constexpr int   m_left_value = 0;
  static constexpr int  m_right_value = 1;

  constexpr side(int  v=0) noexcept: m_data(v){}

  constexpr bool  operator==(side  rhs) const noexcept{return m_data == rhs.m_data;}
  constexpr bool  operator!=(side  rhs) const noexcept{return m_data != rhs.m_data;}

  constexpr bool  is_left()  const noexcept{return m_data == m_left_value;}
  constexpr bool  is_right() const noexcept{return m_data == m_right_value;}

  constexpr side  get_opposite() const noexcept{return side(is_left()? m_right_value:m_left_value);}

};


namespace sides{
constexpr side   left = side(side::m_left_value);
constexpr side  right = side(side::m_right_value);
}




class character;


struct
field
{
  side  m_side;

  gbstd::real_point  m_base_point;

  bar  m_hp_bar;

  int  m_last_hp=0;
  int  m_last_hp_max=0;

  const character*  m_character=nullptr;

  field() noexcept;

  static void  draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, field&  f) noexcept;
  static void  tick(gbstd::task_control  ctrl,                           field&  f) noexcept;

};




}}




#endif




