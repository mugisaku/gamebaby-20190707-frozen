#ifndef ww_notifier_hpp_is_included
#define ww_notifier_hpp_is_included


#include"libww/ww_entry.hpp"
#include"libww/ww_bar.hpp"


namespace ww{




class
notifier
{
  double  m_y_vector;

  double  m_x_pos;
  double  m_y_pos;

  gbstd::point  m_start_pos;
  gbstd::point  m_offset;

  std::string  m_text;

public:
  notifier(gbstd::point  offset, gbstd::point  start_pos, const char*  s) noexcept;

  bool  is_halted() const noexcept{return m_text.empty();}

  void  step() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};


}




#endif




