#ifndef ww_notifier_hpp_is_included
#define ww_notifier_hpp_is_included


#include"libww/ww_entry.hpp"
#include"libww/ww_bar.hpp"


namespace ww{




class
notifier
{
  gbstd::fixed_t  m_y_vector;

  gbstd::fixed_t  m_x_pos;
  gbstd::fixed_t  m_y_pos;

  real_point  m_start_pos;
  real_point  m_offset;

  std::string  m_text;

public:
  notifier(real_point  offset, real_point  start_pos, const char*  s) noexcept;

  bool  is_halted() const noexcept{return m_text.empty();}

  void  step() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};


}




#endif




