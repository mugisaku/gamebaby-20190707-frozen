#ifndef gbstd_window_hpp_was_included
#define gbstd_window_hpp_was_included


#include"libgbstd/image.hpp"
#include<memory>
#include<cstdio>


namespace gbstd{


class window_manager;
class window;
class widget;


struct
window_style
{
  color  colors[4] = {color()                     ,
                      colors::blue      ,
                      colors::white     ,
                      colors::light_gray,};

};


class
window_id
{
  const window*  m_pointer=nullptr;

public:
  constexpr window_id() noexcept{}
  constexpr window_id(const window&  w) noexcept: m_pointer(&w){}

  constexpr bool  operator==(window_id  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
  constexpr bool  operator==(const window&  w) const noexcept{return m_pointer == &w;}

  constexpr bool  operator!=(window_id  rhs) const noexcept{return m_pointer != rhs.m_pointer;}
  constexpr bool  operator!=(const window&  w) const noexcept{return m_pointer != &w;}

};


class
notifier
{
  bool*  m_data;

public:
  notifier(bool&  b) noexcept: m_data(&b){}

  void  touch() const noexcept{*m_data = true;}

};


class
window
{
  std::string  m_name;

  image  m_frame_image;
  image  m_content_image;

  std::unique_ptr<widget>  m_root;

  widget*  m_current=nullptr;

  point  m_position;

  int  m_width;
  int  m_height;

  std::vector<widget*>  m_redraw_queue;

  bool  m_whether_received_reform_request=true;
  bool  m_whether_need_to_total_redraw=true;

  color  m_background_color;

  uint32_t  m_update_counter=0;

  void  process_by_mouse_position(point&  pt) noexcept;

public:
  window(const char*  name="") noexcept;
  window(const window& )=delete;
  window(      window&&)=delete;

  window&  operator=(const window& )=delete;
  window&  operator=(      window&&)=delete;

  const std::unique_ptr<widget>&  get_root() noexcept{return m_root;}

  const std::string&  get_name() const noexcept{return m_name;}

  void          set_position(point  pt)       noexcept{       m_position = pt;}
  const point&  get_position(         ) const noexcept{return m_position     ;}

  bool  test_by_point(int  x, int  y) const noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  void  set_root_widget(widget*  wg) noexcept;

  color  get_background_color() const noexcept{return m_background_color;}
  void   set_background_color(color  color) noexcept;

  void  push_redraw(widget&  wg) noexcept{m_redraw_queue.emplace_back(&wg);}

  void  receive_reform_request() noexcept{m_whether_received_reform_request = true;}

        image&  get_content_image()       noexcept{return m_content_image;}
  const image&  get_content_image() const noexcept{return m_content_image;}

  const widget*  get_current_widget() const noexcept{return m_current;}

  void  do_total_reform_if_necessary() noexcept;
  bool           redraw_if_necessary() noexcept;

  void  process_user_input(point  pt) noexcept;

  uint32_t  get_update_ccounter() const noexcept{return m_update_counter;}

  void  print() const noexcept;

};


class
window_manager
{
  std::vector<std::unique_ptr<window>>  m_window_stack;

public:
  window_manager() noexcept;
  window_manager(const window_manager& )=delete;
  window_manager(      window_manager&&)=delete;
 ~window_manager(){clear();}

  window_manager&  operator=(const window_manager& )=delete;
  window_manager&  operator=(      window_manager&&)=delete;


  void  clear()  noexcept;

  window*  append(window*  w, int  x, int  y) noexcept;
  window*  remove(window*  w) noexcept;

  void  put_down_window_all() const noexcept;

  void  update() noexcept;

  bool  composite(image&  dst) noexcept;

  void  print() const noexcept;

};




class
style_box
{
  point  m_base_position;

  int  m_content_width =0;
  int  m_content_height=0;

  int  m_left_padding  =0;
  int  m_top_padding   =0;
  int  m_right_padding =0;
  int  m_bottom_padding=0;

public:
  style_box(int  w, int  h) noexcept: m_content_width(w), m_content_height(h){}

  const point&  get_base_position() const noexcept{return m_base_position;}

  void  set_base_position(int  x, int  y) noexcept{m_base_position  = point(x,y);}
  void  set_base_position(point  pt)      noexcept{m_base_position  = pt;}
  void  add_base_position(int  x, int  y) noexcept{m_base_position += point(x,y);}
  void  add_base_position(point  pt)      noexcept{m_base_position += pt;}

  int  get_top_padding()    const noexcept{return m_top_padding   ;}
  int  get_left_padding()   const noexcept{return m_left_padding  ;}
  int  get_right_padding()  const noexcept{return m_right_padding ;}
  int  get_bottom_padding() const noexcept{return m_bottom_padding;}

  void  set_top_padding(   int  v) noexcept{m_top_padding    = v;}
  void  set_left_padding(  int  v) noexcept{m_left_padding   = v;}
  void  set_right_padding( int  v) noexcept{m_right_padding  = v;}
  void  set_bottom_padding(int  v) noexcept{m_bottom_padding = v;}

  int  get_content_top()  const noexcept{return m_base_position.y+ m_top_padding;}
  int  get_content_left() const noexcept{return m_base_position.x+m_left_padding;}

  point  get_content_position() const noexcept{return point(get_content_left(),get_content_top());}

  int  get_content_width()  const noexcept{return m_content_width ;}
  int  get_content_height() const noexcept{return m_content_height;}

  rectangle  get_content_rectangle() const noexcept{return rectangle(get_content_left(),
                                                                     get_content_top(),
                                                                     get_content_width(),
                                                                     get_content_height());}


  void  set_content_width( int  v) noexcept{m_content_width  = v;}
  void  set_content_height(int  v) noexcept{m_content_height = v;}

  int  get_width()  const noexcept{return m_left_padding+m_content_width+m_right_padding;}
  int  get_height() const noexcept{return m_top_padding+m_content_height+m_bottom_padding;}

  void  print() const noexcept{
    printf("top-padding:%d,\nleft-padding:%d,\nright-padding:%d,\nbottom-padding:%d,\n",m_top_padding,m_left_padding,m_right_padding,m_bottom_padding);
  }

};


class
widget: public style_box
{
  struct flags{
    static constexpr uint32_t         hidden =  2;
    static constexpr uint32_t      displayed =  4;
    static constexpr uint32_t         frozen =  8;
    static constexpr uint32_t  redraw_queued = 16;
    static constexpr uint32_t     autonomous = 32;

  };


  uint32_t  m_state=0;

  void    set_flag(uint32_t  flag) noexcept{m_state |=  flag;}
  void  unset_flag(uint32_t  flag) noexcept{m_state &= ~flag;}
  bool   test_flag(uint32_t  flag) const noexcept{return m_state&flag;}

  enum class follow_style{
    none,
    right,
    bottom,

  } m_follow_style=follow_style::none;


  window*  m_window=nullptr;

  point  m_offset;

  void*  m_userdata=nullptr;

  std::string  m_name;

  std::vector<std::unique_ptr<widget>>  m_children;

  widget*  m_solo_widget=nullptr;

public:
  widget(int  w=1, int  h=1) noexcept;
  widget(const widget&   rhs) noexcept=delete;
  widget(      widget&&  rhs) noexcept=delete;
  virtual ~widget(){m_children.clear();}

  widget&  operator=(const widget&   rhs) noexcept=delete;
  widget&  operator=(      widget&&  rhs) noexcept=delete;


  virtual const char*  get_class_name() const noexcept{return "widget";}

  const std::string&  get_name() const noexcept{return m_name;}


  bool  is_frozen()     const noexcept{return test_flag(flags::frozen);}
  bool  is_hidden()     const noexcept{return test_flag(flags::hidden);}
  bool  is_displayed()  const noexcept{return test_flag(flags::displayed);}
  bool  is_autonomous() const noexcept{return test_flag(flags::autonomous);}

  void  be_autonomous() noexcept{set_flag(flags::autonomous);}

  void    freeze() noexcept{  set_flag(flags::frozen);}
  void  unfreeze() noexcept{unset_flag(flags::frozen);}

  void    display() noexcept{  set_flag(flags::displayed);}
  void  undisplay() noexcept{unset_flag(flags::displayed);}

  void  show() noexcept{unset_flag(flags::hidden);}
  void  hide() noexcept{  set_flag(flags::hidden);}


  void  set_offset(int  x, int  y) noexcept{m_offset = point(x,y);}
  void  set_offset(point  pt)      noexcept{m_offset  = pt;}
  void  add_offset(int  x, int  y) noexcept{m_offset += point(x,y);}
  void  add_offset(point  pt)      noexcept{m_offset += pt;}


  bool  test_by_point(point  pt) const noexcept;
  widget*  find_by_point(point  pt) noexcept;

  void    set_solo_widget_by_name(const char*  name) noexcept;
  void  unset_solo_widget() noexcept{m_solo_widget = nullptr;}


  void  request_redraw() noexcept;
  void  request_reform() noexcept;


  void  reform(point  parent_content_position) noexcept;
  void  redraw(const image&  img) noexcept;

  virtual void  do_on_mouse_enter() noexcept;
  virtual void  do_on_mouse_leave() noexcept;
  virtual void  do_on_mouse_act(point  mouse_pos) noexcept;

  virtual void  render(const canvas&  cv) noexcept{}


  template<typename  T>T*  get_userdata() const noexcept{return static_cast<T*>(m_userdata);}
  void*  get_userdata() const noexcept{return m_userdata;}

  void  set_userdata(void*  ptr) noexcept
  {
    m_userdata = ptr;
  }

  template<typename  T>  void  set_userdata(T*  ptr) noexcept
  {
    m_userdata = ptr;
  }


  virtual void  print(int  indent=0) const noexcept;


  void     set_window(window*  w)       noexcept;
  window*  get_window(          ) const noexcept{return m_window;}

  void  append_child(widget*  child, int  x, int  y) noexcept;

  void  append_column_child(widget*  child) noexcept;
  void  append_column_child(std::initializer_list<widget*>  children) noexcept;
  void  append_row_child(   widget*  child) noexcept;
  void  append_row_child(std::initializer_list<widget*>  children) noexcept;

};


inline widget*
make_column(std::initializer_list<widget*>  children) noexcept
{
  auto  wg = new widget;

  wg->append_column_child(children);

  return wg;
}


inline widget*
make_row(std::initializer_list<widget*>  children) noexcept
{
  auto  wg = new widget;

  wg->append_row_child(children);

  return wg;
}


}


#endif




