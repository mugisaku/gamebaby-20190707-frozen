#ifndef ww_context_hpp_is_included
#define ww_context_hpp_is_included


#include"libww/ww_section.hpp"




namespace ww{




class
context
{
  static constexpr int  m_screen_width  = 288;
  static constexpr int  m_screen_height = 288;

  uint32_t  m_spilling_text_counter=0;

  gbstd::color  m_background_color;

  section  m_section;

  party  m_left_party;
  party  m_right_party;


  static void  start_display_logo(gbstd::execution&  exec, context&  ctx) noexcept;
  static void   wait_display_logo(gbstd::execution&  exec, context&  ctx) noexcept;

  static void  run(gbstd::execution&  exec, context&  ctx) noexcept;

public:
  context() noexcept;

  bool  has_active_spilling_text() const noexcept{return m_spilling_text_counter;}

  void  push_spilling_text(gbstd::color  color, std::u16string_view  sv, gbstd::point  center, uint32_t  time) noexcept;

  gbstd::execution_entry  make_entry() noexcept{return {"",run,*this};}

  static constexpr int  get_screen_width()  noexcept{return m_screen_width ;}
  static constexpr int  get_screen_height() noexcept{return m_screen_height;}

  static void  tick(                          context&  ctx) noexcept;
  static void  draw(const gbstd::canvas&  cv, context&  ctx) noexcept;

};


}




#endif




