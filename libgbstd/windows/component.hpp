#ifndef gbstd_window_component_hpp_was_included
#define gbstd_window_component_hpp_was_included


#include"libgbstd/window.hpp"


namespace gbstd{




template<typename  T>
class
event
{
  T*  m_caller;

  int  m_kind;

public:
  template<typename  E>constexpr event(T&  caller, E  kind) noexcept: m_caller(&caller), m_kind(static_cast<int>(kind)){}

  constexpr T*  operator->() const noexcept{return  m_caller;}
  constexpr T&  operator*()  const noexcept{return *m_caller;}

  template<typename  E>constexpr bool  test_kind(E  k) const noexcept{return m_kind == static_cast<int>(k);}

  constexpr int  get_value() const noexcept{return m_kind;}

};




class
frame: public widget
{
  std::u16string  m_text;

  widget*  m_container=nullptr;

  character_color  m_character_color;

  color  m_line_color;

  static character_color  m_default_character_color;
  static color            m_default_line_color;

public:
  frame(const char*  text, widget*  wg=nullptr, int  x=0, int  y=0) noexcept;

  const char*  get_class_name() const noexcept override{return "frame";}

  void  insert_content(widget*  wg, int  x, int  y) noexcept;

  void  set_text(const char*  s) noexcept;
  void  set_line_color(color  new_color) noexcept;

  void  render(const canvas&  cv) noexcept override;

  static void  set_default_line_color(color  new_color) noexcept{m_default_line_color = new_color;}

};


class
label: public widget
{
  static character_color  m_default_color;

  character_color  m_color;

  std::u16string  m_text;

public:
  label(const char*      s=""                            ) noexcept;
  label(const char*      s, const character_color&  color) noexcept;
  label(const char16_t*  s                               ) noexcept;
  label(const char16_t*  s, gbstd::color  color          ) noexcept;
  label(const char16_t*  s, const character_color&  color) noexcept;

  const char*  get_class_name() const noexcept override{return "label";}

  static const character_color&  get_character_color() noexcept{return m_default_color;}

  void  set_text(const char*      s) noexcept;
  void  set_text(const char16_t*  s) noexcept;

  void  modify_text(const char*      s) noexcept;
  void  modify_text(const char16_t*  s) noexcept;

  void  set_color(const character_color&  color) noexcept;

  void  render(const canvas&  cv) noexcept override;

};


class button;


class
button_event: public event<button>
{
public:
  using event::event;

  enum class kind{
    release,
    press,
  };

  constexpr bool  is_press()   const noexcept{return test_kind(kind::press  );}
  constexpr bool  is_release() const noexcept{return test_kind(kind::release);}

};


class
button: public widget
{
  widget*  m_container=nullptr;

  void  (*m_callback)(button_event)=nullptr;

  enum class state{
    released,
     pressed,
  } m_state=state::released;


public:
  button(widget*  target, void  (*callback)(button_event)) noexcept;

  const char*  get_class_name() const noexcept override{return "button";}

  bool  is_pressed()  const noexcept{return m_state ==  state::pressed;}
  bool  is_released() const noexcept{return m_state == state::released;}

  void  do_on_mouse_leave() noexcept override;
  void  do_on_mouse_act(point  mouse_pos) noexcept override;

  void  render(const canvas&  cv) noexcept override;

  void  print(int  indent=0) const noexcept override;

};




constexpr int  g_icon_size = 16;


class
icon
{
  color  m_data[g_icon_size][g_icon_size];

public:
  icon(std::initializer_list<int>  ls) noexcept;

  color  get_color(int  x, int  y) const noexcept{return m_data[y][x];}

};


namespace icons{
extern const icon    checked;
extern const icon  unchecked;

extern const icon    radio_checked;
extern const icon  radio_unchecked;

extern const icon         up;
//  static const icon  sunken_up;

extern const icon         left;
extern const icon  sunken_left;

extern const icon         right;
extern const icon  sunken_right;

extern const icon         down;
//  static const icon  sunken_down;

extern const icon   plus;
extern const icon  minus;
}




class iconshow;


class
iconshow_event: public event<iconshow>
{
public:
  using event::event; 

  enum class kind{
    enter,
    leave,
    click,
  };

  bool  is_enter() const noexcept{return test_kind(kind::enter);}
  bool  is_leave() const noexcept{return test_kind(kind::leave);}
  bool  is_click() const noexcept{return test_kind(kind::click);}

};


class
iconshow: public widget
{
  std::vector<const icon*>  m_icons;

  int  m_index=0;

  bool  m_whether_touched=false;

  void  (*m_callback)(iconshow_event  evt)=nullptr;

public:
  iconshow(std::initializer_list<const icon*>  ls={}, void  (*cb)(iconshow_event)=nullptr) noexcept:
  widget(g_icon_size,g_icon_size){assign(ls,cb);}

  const icon*  operator*() const noexcept{return m_icons[m_index];}

  int   get_index(      ) const noexcept{return m_index;}
  void  set_index(int  i)       noexcept;

  void  assign(std::initializer_list<const icon*>  ls={}, void  (*cb)(iconshow_event)=nullptr) noexcept;

  const char*  get_class_name() const noexcept override{return "iconshow";}

  void  do_on_mouse_enter() noexcept override{if(m_callback){m_callback({*this,iconshow_event::kind::enter});}}
  void  do_on_mouse_leave() noexcept override;
  void  do_on_mouse_act(point  mouse_pos) noexcept override;

  void  render(const canvas&  cv) noexcept override;

};




class checkbox;


class
checkbox_event: public event<checkbox>
{
public:
  enum class kind{
      set,
    unset,
  };

  using event::event;

  bool  is_set()   const noexcept{return test_kind(kind::set);}
  bool  is_unset() const noexcept{return test_kind(kind::unset);}

};




class
checkbox: public widget
{
  struct shared_data;

  shared_data*  m_data;

  iconshow*  m_iconshow;
  label*     m_label;

  uint32_t  m_entry_number=0;

  checkbox*  m_next=nullptr;

  void*  get_common_userdata_internal() const noexcept;

  static void  iconshow_callback(      iconshow_event  evt) noexcept;
  static void  iconshow_radio_callback(iconshow_event  evt) noexcept;

protected:
  void  revise_to_radio() noexcept;

public:
  checkbox(                         const char16_t*  text, void  (*callback)(checkbox_event)) noexcept;
  checkbox(const checkbox&  member, const char16_t*  text) noexcept;
 ~checkbox();

  void    check() noexcept;
  void  uncheck() noexcept;

  const char*  get_class_name() const noexcept override{return "checkbox";}

  bool  is_checked() const noexcept{return m_iconshow->get_index();}

  operator bool() const noexcept{return is_checked();}

  uint32_t  get_entry_number() const noexcept{return m_entry_number;}

  void  set_common_userdata(void*  ptr) const noexcept;

  template<typename  T>
  T*  get_common_userdata() const noexcept{return static_cast<T*>(get_common_userdata_internal());}

};




class
radio_button: public checkbox
{
public:
  radio_button(                             const char16_t*  text, void  (*callback)(checkbox_event)) noexcept;
  radio_button(const radio_button&  member, const char16_t*  text) noexcept;

  const char*  get_class_name() const noexcept override{return "radio_button";}

};




class dial;


class
dial_event: public event<dial>
{
public:
  using event::event;

  int  get_new_value() const noexcept;
  int  get_old_value() const noexcept{return get_value();}

};


class
dial: public widget
{
  int  m_current=0;

  int  m_min=0;
  int  m_max=0;

  void  (*m_callback)(dial_event  evt)=nullptr;

  iconshow*     m_up_show;
  iconshow*   m_down_show;

  button*     m_up_button;
  button*   m_down_button;

  label*   m_label;

  static void    up(button_event  evt) noexcept;
  static void  down(button_event  evt) noexcept;

  void  update_label() noexcept;

public:
  dial(int  min, int  max, void  (*callback)(dial_event  evt)) noexcept;

  void  set_current(int  v) noexcept;

  int  get_current() const noexcept{return m_current;}

  int  get_min() const noexcept{return m_min;}
  int  get_max() const noexcept{return m_max;}

};




class
item_cursor
{
  point  m_base  ;
  point  m_offset;

public:
  constexpr item_cursor() noexcept{}
  constexpr item_cursor(point  base, point  off) noexcept: m_base(base), m_offset(off){}

  constexpr item_cursor  operator+(point  pt) const noexcept
  {
    return item_cursor(m_base,m_offset+pt);
  }

  constexpr const point&    get_base() const noexcept{return m_base;}
  constexpr const point&  get_offset() const noexcept{return m_offset;}

  void    add_base(int  x, int   y) noexcept{m_base   += point(x,y);}
  void  add_offset(int  x, int   y) noexcept{m_offset += point(x,y);}

  constexpr point  get_point() const noexcept{return m_base+m_offset;}

};


template<typename  T>
class
item_table_shell
{
public:
  virtual int  get_x_length() const noexcept=0;
  virtual int  get_y_length() const noexcept=0;

  virtual int  get_item_width()  const noexcept=0;
  virtual int  get_item_height() const noexcept=0;

  virtual void  render(item_cursor  cur, const canvas&  cv) const noexcept;

  int  get_content_width()  const noexcept{return get_item_width() *get_x_length();}
  int  get_content_height() const noexcept{return get_item_height()*get_y_length();}

};


template<typename  T>class  menu;


template<typename  T>
class
menu_event: public event<menu<T>>
{
public:
  using event<menu<T>>::event;

  enum class kind{
    move,
  };

  bool  is_move() const noexcept{return test_kind(kind::move);}

};


template<typename  T>
class
menu: public widget
{
  item_table_shell<T>  m_shell;

  int  m_number_of_columns=0;
  int  m_number_of_rows=0;

  item_cursor  m_cursor;

  bool  m_whether_show_cursor=true;

public:
  using callback = void(*)(menu_event<T>  evt);

private:
  callback  m_callback=nullptr;

  void  call(menu_event<T>  k) noexcept{if(m_callback){m_callback(*this,k);}}

public:
  menu() noexcept{}
  menu(const item_table_shell<T>&  shell, int  ncols, int  nrows) noexcept{reset(shell,ncols,nrows);}

  int  get_number_of_columns() const noexcept{return m_number_of_columns;}
  int  get_number_of_rows()    const noexcept{return m_number_of_rows;}

  int  get_item_width()  const noexcept{return m_shell.get_item_width() ;}
  int  get_item_height() const noexcept{return m_shell.get_item_height();}

  const item_table_shell<T>&  get_shell() const noexcept{return m_shell;}

  void  resize(int  ncols, int  nrows) noexcept
  {
    m_number_of_columns = ncols;
    m_number_of_rows    = nrows;

    m_cursor = item_cursor();

    call(menu_event<T>::kind::move);

    set_content_width( m_shell.get_content_width() *ncols);
    set_content_height(m_shell.get_content_height()*nrows);

    request_reform();
  }


  void  reset(const item_table_shell<T>&  shell, int  ncols, int  nrows) noexcept
  {
    m_shell = shell;

    resize(ncols,nrows);
  }

  bool  does_show_cursor() const noexcept{return m_whether_show_cursor;}
  void  show_cursor() noexcept{m_whether_show_cursor =  true;}
  void  hide_cursor() noexcept{m_whether_show_cursor = false;}

  const item_cursor&  get_cursor()  const noexcept{return m_cursor;}

  void  move_up() noexcept
  {
      if(m_cursor.get_offset().y)
      {
        m_cursor.add_offset(0,-1);

        call(menu_event<T>::kind::move);

        request_redraw();
      }

    else
      if(m_cursor.get_base().y)
      {
        m_cursor.add_base(0,-1);

        call(menu_event<T>::kind::move);

        request_redraw();
      }
  }

  void  move_down() noexcept
  {
      if(m_cursor.get_offset().y < (m_number_of_rows-1))
      {
        m_cursor.add_offset(0,1);

        call(menu_event<T>::kind::move);

        request_redraw();
      }

    else
      if((m_cursor.get_base().y+m_number_of_rows) < (m_shell.get_y_length()-1))
      {
        m_cursor.add_base(0,1);

        call(menu_event<T>::kind::move);

        request_redraw();
      }
  }

  void  move_left() noexcept
  {
      if(m_cursor.get_offset().x)
      {
        m_cursor.add_offset(-1,0);

        call(menu_event<T>::kind::move);

        request_redraw();
      }

    else
      if(m_cursor.get_base().x)
      {
        m_cursor.add_base(-1,0);

        call(menu_event<T>::kind::move);

        request_redraw();
      }
  }

  void  move_right() noexcept
  {
      if(m_cursor.get_offset().x < (m_number_of_columns-1))
      {
        m_cursor.add_offset(1,0);

        call(menu_event<T>::kind::move);

        request_redraw();
      }

    else
      if((m_cursor.get_base().x+m_number_of_columns) < (m_shell.get_x_length()-1))
      {
        m_cursor.add_base(1,0);

        call(menu_event<T>::kind::move);

        request_redraw();
      }
  }

  void  render(const canvas&  cv) noexcept override
  {
    int  item_w = m_shell.get_item_width() ;
    int  item_h = m_shell.get_item_height();

      for(int  y = 0;  y < m_number_of_rows   ;  ++y){
      for(int  x = 0;  x < m_number_of_columns;  ++x){
        canvas  cocv(cv,item_w*x,item_h*y,
                        item_w  ,item_h  );

        m_shell.render(m_cursor+point(x,y),cocv);
      }}


      if(does_show_cursor())
      {
        point  pt(item_w*m_cursor.get_offset().x,
                  item_h*m_cursor.get_offset().y);

        render_cursor(pt,cv);
      }
  }


  virtual void  render_cursor(point  item_pos, const canvas&  cv) const noexcept
  {
    int  w = m_shell.get_item_width() ;
    int  h = m_shell.get_item_height();

    cv.draw_rectangle(colors::yellow,item_pos.x,item_pos.y,w,h);
  }

};


}


#endif




