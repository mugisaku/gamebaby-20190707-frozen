#ifndef ww_notifier_hpp_is_included
#define ww_notifier_hpp_is_included


#include"libww/ww_entry.hpp"
#include"libww/ww_bar.hpp"


namespace ww{




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


}




#endif




