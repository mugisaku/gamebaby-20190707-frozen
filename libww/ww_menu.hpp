#ifndef ww_menu_hpp_is_included
#define ww_menu_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include<list>
#include<vector>




namespace ww{
namespace menus{




class
entry
{
  void*  m_data=nullptr;

  bool  m_active_flag=false;
  bool  m_selected_flag=false;

public:
  entry() noexcept{}

  template<typename  T>
  entry(T&  data, bool  active_flag=true) noexcept:
  m_data(&data), m_active_flag(active_flag){}

  template<typename  T>
  T&  get_data() const noexcept{return *static_cast<T*>(m_data);}

  operator bool() const noexcept{return m_active_flag;}

  bool  test_active_flag() const noexcept{return m_active_flag;}

  void    set_active_flag() noexcept{m_active_flag =  true;}
  void  unset_active_flag() noexcept{m_active_flag = false;}

  bool  is_selected() const noexcept{return m_selected_flag;}

  void    select() noexcept{m_selected_flag =  true;}
  void  unselect() noexcept{m_selected_flag = false;}

};


class
table
{
  std::vector<entry>  m_container;

  int   m_width =0;
  int   m_height=0;

  int   m_entry_width =0;
  int   m_entry_height=0;

public:
  table() noexcept{}

  table&  resize(int  w, int  h) noexcept;

  table&  set_entry_width( int  n) noexcept{  m_entry_width  = n;  return *this;}
  table&  set_entry_height(int  n) noexcept{  m_entry_height = n;  return *this;}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int  get_entry_width()  const noexcept{return m_entry_width ;}
  int  get_entry_height() const noexcept{return m_entry_height;}

  entry*  get_entry_pointer(int  x, int  y) noexcept{return &m_container[m_width*y+x];}

  void  draw(int  x, int  y, int  w, int  h, void  (*cb)(const entry&,gbstd::dummy&,const gbstd::canvas&), const gbstd::canvas&  cv) noexcept;

};


struct
window: public gbstd::rectangle
{
  gbstd::color  m_color;

};


class view;


class
cursor
{
  view*  m_view=nullptr;

  bool  m_visible=false;

  gbstd::point  m_point;

public:
  cursor&  reset(view&  v) noexcept;

  bool  is_visible() const noexcept{return m_visible;}

  cursor&  set_x(int  n) noexcept;
  cursor&  add_x(int  n) noexcept;
  cursor&  set_y(int  n) noexcept;
  cursor&  add_y(int  n) noexcept;

  const gbstd::point&  get_point() const noexcept{return m_point;}

  cursor&  show() noexcept{  m_visible =  true;  return *this;}
  cursor&  hide() noexcept{  m_visible = false;  return *this;}

};


class
view
{
  table*  m_table=nullptr;

  int   m_width =0;
  int   m_height=0;

  gbstd::point  m_offset;

  cursor  m_first_cursor;
  cursor  m_second_cursor;

  window  m_window;

  void  (*m_callback)(const entry&,gbstd::dummy&,const gbstd::canvas&  cv)=nullptr;

  void  draw_cursor(const cursor&  cur, const gbstd::canvas&  cv) noexcept;

public:
  view() noexcept{}
  view(table&  tbl) noexcept{assign(tbl);}

  view&  oprator(table&  tbl) noexcept{return assign(tbl);}

  view&  assign(table&  tbl) noexcept;

  view&  set_x(int  n) noexcept{  m_window.x = n;  return *this;}
  view&  set_y(int  n) noexcept{  m_window.y = n;  return *this;}
  view&  set_window_color(gbstd::color  color) noexcept{  m_window.m_color = color;  return *this;}
  view&  set_width( int  n) noexcept;
  view&  set_height(int  n) noexcept;

  cursor&   get_first_cursor() noexcept{return  m_first_cursor;}
  cursor&  get_second_cursor() noexcept{return m_second_cursor;}

  const table&  get_table() const noexcept{return *m_table;}

  view&  set_x_offset(int  n) noexcept;
  view&  add_x_offset(int  n) noexcept;
  view&  set_y_offset(int  n) noexcept;
  view&  add_y_offset(int  n) noexcept;

  template<typename  T>
  view&  set_callback(void  (*cb)(const entry&,T&,const gbstd::canvas&)) noexcept{
    m_callback = reinterpret_cast<void(*)(const entry&,gbstd::dummy&,const gbstd::canvas&)>(cb);  return *this;
  }

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  const window&  get_window() const noexcept{return m_window;}

  void  draw(const gbstd::canvas&  cv) noexcept;

};


class
stack
{
  struct element{
    table*  m_table;
    view*    m_view;

  };

  std::vector<element>  m_container;

public:

};


}}




#endif




