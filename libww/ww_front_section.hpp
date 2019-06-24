#ifndef ww_front_section_hpp_is_included
#define ww_front_section_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include"libww/ww_opening_section.hpp"
#include<list>
#include<vector>




namespace ww{



class system;


class
front_section
{
  friend class system;

  system&  m_system;

  gbstd::clock  m_clock;

  gbstd::task  m_task;

  int  m_phase;

  union{
    opening_section  m_opening_section;
  };

  static void  run(gbstd::execution&  exec, front_section&  fr) noexcept;

  static void  tick(                          front_section&  fr) noexcept;
  static void  draw(const gbstd::canvas&  cv, front_section&  fr) noexcept;

public:
  front_section(system&  sys, gbstd::execution&  exec) noexcept;
 ~front_section();

};


}




#endif




