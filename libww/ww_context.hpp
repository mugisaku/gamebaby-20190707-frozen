#ifndef ww_context_hpp_is_included
#define ww_context_hpp_is_included


#include"libww/ww_front_section.hpp"




namespace ww{




class system;


class
context
{
  friend class system;

  system&  m_system;

  gbstd::clock  m_clock;

  static constexpr int  m_screen_width  = 288;
  static constexpr int  m_screen_height = 288;

  union{
    front_section  m_front_section;
  };

  party  m_left_party;
  party  m_right_party;

  static void  start_display_logo(gbstd::execution&  exec, context&  ctx) noexcept;
  static void   wait_display_logo(gbstd::execution&  exec, context&  ctx) noexcept;

  static void  run(gbstd::execution&  exec, context&  ctx) noexcept;

  context(system&  sys) noexcept;

public:
  gbstd::execution_entry  make_entry() noexcept{return {"",run,*this};}

  static constexpr int  get_screen_width()  noexcept{return m_screen_width ;}
  static constexpr int  get_screen_height() noexcept{return m_screen_height;}

};


}




#endif




