#ifndef ww_menu_hpp_is_included
#define ww_menu_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include<list>
#include<vector>




namespace ww{




struct
window: public gbstd::rectangle
{
  gbstd::color  m_color;

};


struct
menu
{
  struct entry{
    std::u16string_view  m_label;

    bool  m_active_flag;

    entry() noexcept{}

    entry(std::u16string_view  label, bool  active_flag=true) noexcept:
    m_label(label), m_active_flag(active_flag){}

    int  get_width() const noexcept{return gbstd::g_font_width*m_label.size();}

  };


  window  m_window;

  int  m_cursor_position;

  std::vector<entry>  m_entry_list;

  void  push_entry(std::initializer_list<menu::entry>  ls) noexcept
  {
      for(auto  ent:  ls)
      {
        m_entry_list.emplace_back(ent);

        m_window.w =  std::max(m_window.w,ent.get_width());
        m_window.h += gbstd::g_font_height;
      }
  }

  static void  draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, menu&  m) noexcept;
  static void  tick(gbstd::task_control  ctrl,                           menu&  m) noexcept;

};


}




#endif




