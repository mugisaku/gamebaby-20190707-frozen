#ifndef ww_spilling_text_hpp_is_included
#define ww_spilling_text_hpp_is_included


#include"libgbstd/instruction_queue.hpp"
#include"libgbstd/task.hpp"


namespace ww{




class
spilling_text
{
  gbstd::fixed_t  m_y_vector;

  gbstd::real_point  m_pos;
  gbstd::real_point  m_bottom_pos;

  gbstd::color  m_color;

  std::string  m_text;

public:
  spilling_text() noexcept{}

  spilling_text&  set_text(std::string_view  text) noexcept{  m_text = text;  return *this;}
  spilling_text&  set_color(gbstd::color  color) noexcept{  m_color = color;  return *this;}

  spilling_text&  reset(gbstd::real_point  bottom_pos, int  offset) noexcept;

  bool  is_finished() const noexcept{return m_text.empty();}

  static void  tick(                          spilling_text&  spltxt) noexcept;
  static void  draw(const gbstd::canvas&  cv, spilling_text&  spltxt) noexcept;

};


}




#endif




