#ifndef ww_manage_section_hpp_is_included
#define ww_manage_section_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include"libgbstd/menu.hpp"
#include"libgbstd/character.hpp"
#include<list>
#include<vector>




namespace ww{




struct
manage_section
{
  uint32_t  m_money=0;

  uint32_t      m_empty_land=0;
  uint32_t      m_waste_land=0;
  uint32_t   m_building_land=0;
  uint32_t       m_farm_land=0;
  uint32_t     m_forest_land=0;
  uint32_t  m_reservoir_land=0;

  uint32_t  m_water=0;
  uint32_t  m_food=0;
  uint32_t  m_wood=0;
  uint32_t  m_fuel=0;


  gbstd::clock  m_clock;
  gbstd::task   m_task;

  gbstd::menus::stack  m_menu_stack;

  gbstd::menus::table  m_first_menu_table;
  gbstd::menus::view   m_first_menu_view;

  gbstd::menus::table  m_second_menu_table;
  gbstd::menus::view   m_second_menu_view;

  gbstd::menus::table  m_third_menu_table;
  gbstd::menus::view   m_third_menu_view;

  gbstd::windows::content  m_face_content;
  gbstd::windows::frame    m_face_frame;

  gbstd::windows::content  m_text_content;
  gbstd::windows::frame    m_text_frame;


  static constexpr int  m_text_table_width  = 24;
  static constexpr int  m_text_table_height =  4;

  char16_t  m_text_buffer[1024];

  const char16_t*   m_text_read_pointer;
        char16_t*  m_text_write_pointer;

  gbstd::text   m_text;
  gbstd::color  m_text_color;

  int  m_text_cursor_position;

  int  m_face_animation_index;

  using menu_view   = gbstd::menus::view;
  using menu_stack  = gbstd::menus::stack;
  using menu_result = gbstd::menus::result;



  static void  draw_face(const gbstd::canvas&  cv, const gbstd::windows::style&  style, manage_section&  man) noexcept;
  static void  draw_text(const gbstd::canvas&  cv, const gbstd::windows::style&  style, manage_section&  man) noexcept;

  static void  manage_first_menu(menu_stack&  stk, const menu_result*  res, menu_view&  view, manage_section&  man) noexcept;

  static void  charactick(manage_section&  man) noexcept;

  bool  has_remain_text() const noexcept{return *m_text_read_pointer;}

  void  initialize_menu() noexcept;

  void  startup(gbstd::execution&  exec) noexcept;

  void  clear_text_queue() noexcept;

  void  push_text(std::u16string_view  sv) noexcept;

  void  clear_and_push_text(std::u16string_view  sv) noexcept;

  void  pump_text() noexcept;

  manage_section(gbstd::execution&  exec) noexcept;

};


}




#endif




