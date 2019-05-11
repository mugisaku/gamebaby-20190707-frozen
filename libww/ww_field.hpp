#ifndef ww_field_hpp_is_included
#define ww_field_hpp_is_included


#include"libww/ww_bar.hpp"




namespace ww{
namespace battles{




constexpr int  g_center_space_width  = 64;
constexpr int  g_cell_width  = 64;
constexpr int  g_cell_height = 48;
constexpr int  g_row_width  = g_cell_width*3;
constexpr int  g_row_height = g_cell_height;
constexpr int  g_column_width  = g_cell_width;
constexpr int  g_column_height = g_cell_height*6;




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
  gbstd::real_point  m_name_point;
  gbstd::real_point  m_hp_point;

  bar  m_hp_bar;

  const character*  m_character=nullptr;

  static void  draw(const gbstd::canvas&  cv, field&  f) noexcept;
  static void  tick(                          field&  f) noexcept;

};




}}




#endif




