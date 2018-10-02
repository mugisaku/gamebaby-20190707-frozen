#ifndef gbstd_window_hpp_was_included
#define gbstd_window_hpp_was_included


#include"libgbstd/image.hpp"
#include<memory>


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
  void    finish_redraw_if_necessary() noexcept;

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
widget
{
  struct flags{
    static constexpr uint32_t         hidden =  2;
    static constexpr uint32_t      displayed =  4;
    static constexpr uint32_t         frozen =  8;
    static constexpr uint32_t  redraw_queued = 16;

  };


  uint32_t  m_state=0;

  void    set_flag(uint32_t  flag) noexcept{m_state |=  flag;}
  void  unset_flag(uint32_t  flag) noexcept{m_state &= ~flag;}
  bool   test_flag(uint32_t  flag) const noexcept{return m_state&flag;}

  window*  m_window=nullptr;

  point  m_absolute_position;
  point  m_relative_position;

  int  m_width =0;
  int  m_height=0;

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


  bool  is_frozen()    const noexcept{return test_flag(flags::frozen);}
  bool  is_hidden()    const noexcept{return test_flag(flags::hidden);}
  bool  is_displayed() const noexcept{return test_flag(flags::displayed);}

  void    freeze() noexcept{  set_flag(flags::frozen);}
  void  unfreeze() noexcept{unset_flag(flags::frozen);}

  void    display() noexcept{  set_flag(flags::displayed);}
  void  undisplay() noexcept{unset_flag(flags::displayed);}

  void  show() noexcept{unset_flag(flags::hidden);}
  void  hide() noexcept{  set_flag(flags::hidden);}


  bool     test_by_absolute_point(point  pt) const noexcept;
  widget*  find_by_absolute_point(point  pt)       noexcept;

  const point&  get_absolute_position() const noexcept{return m_absolute_position;}
  const point&  get_relative_position() const noexcept{return m_relative_position;}

  void  set_relative_position(int  x, int  y) noexcept{m_relative_position = point(x,y);}
  void  set_relative_position(point  pt) noexcept{m_relative_position  = pt;}
  void  add_relative_position(int  x, int  y) noexcept{m_relative_position += point(x,y);}
  void  add_relative_position(point  pt) noexcept{m_relative_position += pt;}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  set_width( int  v) noexcept{ m_width = v;}
  void  set_height(int  v) noexcept{m_height = v;}


  void    set_solo_widget_by_name(const char*  name) noexcept;
  void  unset_solo_widget() noexcept{m_solo_widget = nullptr;}


  void  request_redraw() noexcept;
  void  request_reform() noexcept;


  void  reform(point  parent_absolute_position) noexcept;
  void  redraw(const image&  img) noexcept;

  virtual void  do_on_mouse_enter() noexcept{}
  virtual void  do_on_mouse_leave() noexcept{}
  virtual void  do_on_mouse_act(point  mouse_pos) noexcept{}

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

};




}


#endif




