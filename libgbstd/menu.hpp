#ifndef gbstd_menu_hpp_is_included
#define gbstd_menu_hpp_is_included


#include"libgbstd/utility.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/clock.hpp"
#include<vector>




namespace gbstd{




namespace windows{


struct
style
{
  int     m_top_width;
  int  m_bottom_width;

  int   m_left_side_width;
  int  m_right_side_width;

  style&  set_top_width(       int  n) noexcept{  m_top_width        = n;  return *this;}
  style&  set_bottom_width(    int  n) noexcept{  m_bottom_width     = n;  return *this;}
  style&  set_left_side_width( int  n) noexcept{  m_left_side_width  = n;  return *this;}
  style&  set_right_side_width(int  n) noexcept{  m_right_side_width = n;  return *this;}

};


class
content
{
  int  m_width =0;
  int  m_height=0;

  void*  m_data=0;

  void  (*m_callback)(const canvas&,const style&,dummy&)=nullptr;

public:
  content() noexcept{}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  content&  set_width( int  n) noexcept{  m_width  = n;  return *this;}
  content&  set_height(int  n) noexcept{  m_height = n;  return *this;}

  template<typename  T>
  content&  set_callback(void  (*cb)(const canvas&,const style&,T&), T&  t) noexcept
  {
    m_data     = &t;
    m_callback = reinterpret_cast<void(*)(const canvas&,const style&,dummy&)>(cb);

    return *this;
  }

  void  draw(const canvas&  cv, const style&  s, int  x, int  y) noexcept;

};


struct
frame_assembler
{
  void  (*m_top       )(const canvas&  cv, const style&  s);
  void  (*m_bottom    )(const canvas&  cv, const style&  s);
  void  (*m_left_side )(const canvas&  cv, const style&  s);
  void  (*m_right_side)(const canvas&  cv, const style&  s);
  void  (*m_background)(const canvas&  cv, const style&  s);

};


extern style            g_default_style;
extern frame_assembler  g_default_frame_assembler;


class
frame
{
  point  m_position;

  content*  m_content_pointer=nullptr;

  const style*            m_style_pointer=&g_default_style;
  const frame_assembler*  m_assembler_pointer=&g_default_frame_assembler;

public:
  frame() noexcept{}

  frame&  set_x(int  n) noexcept{  m_position.x = n;  return *this;}
  frame&  set_y(int  n) noexcept{  m_position.y = n;  return *this;}

  int  get_x() const noexcept{return m_position.x;}
  int  get_y() const noexcept{return m_position.y;}

  int  get_width()  const noexcept;
  int  get_height() const noexcept;

  content*  get_content(              ) const noexcept{return  m_content_pointer                      ;}
  frame&    set_content(content*  cont)       noexcept{        m_content_pointer = cont;  return *this;}

  const style&  get_style(                   ) const noexcept{return *m_style_pointer                        ;}
  frame&        set_style(const style&  style)       noexcept{        m_style_pointer = &style;  return *this;}

  const frame_assembler*  get_assembler(                          ) const noexcept{return  m_assembler_pointer                    ;}
  frame&                  set_assembler(const frame_assembler*  as)       noexcept{        m_assembler_pointer = as;  return *this;}

  void  draw(const canvas&  cv) const noexcept;

};


}




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


class view;


class
cursor
{
  view*  m_view=nullptr;

  bool  m_visible=false;

  gbstd::point  m_position;

public:
  cursor&  reset(view&  v) noexcept;

  bool  is_visible() const noexcept{return m_visible;}

  cursor&  set_x(int  n) noexcept;
  cursor&  add_x(int  n) noexcept;
  cursor&  set_y(int  n) noexcept;
  cursor&  add_y(int  n) noexcept;

  cursor&  set_position(point  pt) noexcept{  set_x(pt.x);  set_y(pt.y);  return *this;}

  template<typename  T>
  T&  get_data() const noexcept;

  int  get_x() const noexcept{return m_position.x;}
  int  get_y() const noexcept{return m_position.y;}

  const gbstd::point&  get_position() const noexcept{return m_position;}

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

  windows::frame    m_frame;
  windows::content  m_content;

  void  (*m_callback)(const entry&,gbstd::dummy&,const gbstd::canvas&  cv)=nullptr;

  void  draw_cursor(const cursor&  cur, const gbstd::canvas&  cv) noexcept;

  bool  m_busy_flag=false;

  static void  draw_content(const canvas&  cv, const windows::style&  style, view&  v) noexcept;

public:
  view() noexcept{}
  view(table&  tbl) noexcept{assign(tbl);}

  view&  operator=(table&  tbl) noexcept{return assign(tbl);}

  view&  assign(table&  tbl) noexcept;

  view&  set_x(int  n) noexcept{  m_frame.set_x(n);  return *this;}
  view&  set_y(int  n) noexcept{  m_frame.set_y(n);  return *this;}
  view&  set_window_color(gbstd::color  color) noexcept{ /* m_window.m_color = color;*/  return *this;}
  view&  set_width( int  n) noexcept;
  view&  set_height(int  n) noexcept;

  bool  test_busy_flag() const noexcept{return m_busy_flag;}

  view&    set_busy_flag() noexcept{  m_busy_flag =  true;  return *this;}
  view&  unset_busy_flag() noexcept{  m_busy_flag = false;  return *this;}

  cursor&   get_first_cursor() noexcept{return  m_first_cursor;}
  cursor&  get_second_cursor() noexcept{return m_second_cursor;}

        table&  get_table()       noexcept{return *m_table;}
  const table&  get_table() const noexcept{return *m_table;}

        windows::frame&  get_window_frame()       noexcept{return m_frame;}
  const windows::frame&  get_window_frame() const noexcept{return m_frame;}

  int  get_x_offset() const noexcept{return m_offset.x;}
  int  get_y_offset() const noexcept{return m_offset.y;}

  const point&  get_offset() const noexcept{return m_offset;}

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

  void  draw(const gbstd::canvas&  cv) noexcept;

};


template<typename  T>
T&
cursor::
get_data() const noexcept
{
  int  x = m_view->get_x_offset()+m_position.x;
  int  y = m_view->get_y_offset()+m_position.y;

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

    std::vector<windows::frame*>  m_subwindow_list;

  };


  std::vector<element>  m_container;

  task_control  m_control;

  stack&  internal_open(int  opening_value, view&  v, void*  data, callback<dummy>  cb) noexcept;

public:
  stack&  clear() noexcept;

  int  get_number_of_elements() const noexcept{return m_container.size();}

  stack&  ready(clock_watch  w, uint32_t  intval, gbstd::task_list&  ls) noexcept;

  template<typename  T>
  stack&  open(int  opening_value, view&  v, T&  data, callback<T>  cb) noexcept
  {
    return internal_open(opening_value,v,&data,reinterpret_cast<callback<dummy>>(cb));
  }

  stack&  add_subwindow(windows::frame&  frm) noexcept;

  stack&  close_top(int  closing_value) noexcept;

  int  get_opening_value() const noexcept{return m_container.back().m_opening_value;}

  static void  draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, stack&  stk) noexcept;
  static void  tick(gbstd::task_control  ctrl,                           stack&  stk) noexcept;

};


}}




#endif




