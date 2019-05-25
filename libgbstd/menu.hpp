#ifndef gbstd_menu_hpp_is_included
#define gbstd_menu_hpp_is_included


#include"libgbstd/utility.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/clock.hpp"
#include<vector>




namespace gbstd{
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

        entry*  get_entry_pointer(int  x, int  y)       noexcept{return &m_container[m_width*y+x];}
  const entry*  get_entry_pointer(int  x, int  y) const noexcept{return &m_container[m_width*y+x];}

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

  template<typename  T>
  T&  get_data() const noexcept;

  int  get_x() const noexcept{return m_point.x;}
  int  get_y() const noexcept{return m_point.y;}

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

  bool  m_busy_flag=false;

public:
  view() noexcept{}
  view(table&  tbl) noexcept{assign(tbl);}

  view&  operator=(table&  tbl) noexcept{return assign(tbl);}

  view&  assign(table&  tbl) noexcept;

  view&  set_x(int  n) noexcept{  m_window.x = n;  return *this;}
  view&  set_y(int  n) noexcept{  m_window.y = n;  return *this;}
  view&  set_window_color(gbstd::color  color) noexcept{  m_window.m_color = color;  return *this;}
  view&  set_width( int  n) noexcept;
  view&  set_height(int  n) noexcept;

  bool  test_busy_flag() const noexcept{return m_busy_flag;}

  view&    set_busy_flag() noexcept{  m_busy_flag =  true;  return *this;}
  view&  unset_busy_flag() noexcept{  m_busy_flag = false;  return *this;}

  cursor&   get_first_cursor() noexcept{return  m_first_cursor;}
  cursor&  get_second_cursor() noexcept{return m_second_cursor;}

        table&  get_table()       noexcept{return *m_table;}
  const table&  get_table() const noexcept{return *m_table;}

  int  get_x_offset() const noexcept{return m_offset.x;}
  int  get_y_offset() const noexcept{return m_offset.y;}

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


template<typename  T>
T&
cursor::
get_data() const noexcept
{
  int  x = m_view->get_x_offset()+m_point.x;
  int  y = m_view->get_y_offset()+m_point.y;

  return m_view->get_table().get_entry_pointer(x,y)->get_data<T>();
}


class
result
{
  int  m_opening_value=0;
  int  m_closing_value=0;

public:
  result(int  op=0, int  cl=0) noexcept:
  m_opening_value(op), m_closing_value(cl){}

  int  get_opening_value() const noexcept{return m_opening_value;}
  int  get_closing_value() const noexcept{return m_closing_value;}

};


class
stack
{
public:
  template<typename  T>
  using callback = void(*)(stack&,const result*,view&,T&);

private:
  struct element{
    int  m_opening_value;

    view*  m_view;
    void*  m_data;

    callback<dummy>  m_callback;

  };


  std::vector<element>  m_container;

  task_control  m_control;

  stack&  internal_open(int  opening_value, view&  v, void*  data, callback<dummy>  cb) noexcept;

public:
  int  get_number_of_elements() const noexcept{return m_container.size();}

  stack&  ready(clock_watch  w, uint32_t  intval, gbstd::task_list&  ls) noexcept;

  template<typename  T>
  stack&  open(int  opening_value, view&  v, T&  data, callback<T>  cb) noexcept
  {
    return internal_open(opening_value,v,&data,reinterpret_cast<callback<dummy>>(cb));
  }

  stack&  close_top(int  closing_value) noexcept;

  int  get_opening_value() const noexcept{return m_container.back().m_opening_value;}

  static void  draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, stack&  stk) noexcept;
  static void  tick(gbstd::task_control  ctrl,                           stack&  stk) noexcept;

};


}}




#endif




