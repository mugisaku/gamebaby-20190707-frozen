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




class label;


class
frame: public widget
{
  widget*  m_container=nullptr;

  label*  m_label;

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

  void  do_on_mouse_act(point  mouse_pos) noexcept override;

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

  constexpr bool  operator==(const item_cursor&  rhs) const noexcept
  {
    return (m_base == rhs.m_base) && (m_offset == rhs.m_offset);
  }

  constexpr bool  operator!=(const item_cursor&  rhs) const noexcept
  {
    return !(*this == rhs);
  }

  constexpr item_cursor  operator+(point  pt) const noexcept
  {
    return item_cursor(m_base,m_offset+pt);
  }

  constexpr const point&    get_base() const noexcept{return m_base;}
  constexpr const point&  get_offset() const noexcept{return m_offset;}
  constexpr point         get_result() const noexcept{return m_base+m_offset;}

  void    add_base(point  pt) noexcept{m_base   += pt;}
  void  add_offset(point  pt) noexcept{m_offset += pt;}

  void    set_base(point  pt) noexcept{m_base   = pt;}
  void  set_offset(point  pt) noexcept{m_offset = pt;}

};


class menu;


class
menu_event: public event<menu>
{
public:
  using event::event;

  enum class kind{
    enter,
    leave,
    press,
    release,
  };

  bool  is_enter() const noexcept{return test_kind(kind::enter);}
  bool  is_leave() const noexcept{return test_kind(kind::leave);}
  bool  is_press() const noexcept{return test_kind(kind::press);}
  bool  is_release() const noexcept{return test_kind(kind::release);}

};


struct
item_table_size
{
  int  x_length;
  int  y_length;

};


struct
item_size
{
  int  width ;
  int  height;

};


class
menu: public widget
{
  item_table_size  m_item_table_size;
  item_size        m_item_size;

  int  m_number_of_columns=0;
  int  m_number_of_rows=0;

  item_cursor  m_cursor;

public:
  using callback = void(*)(menu_event  evt);

private:
  callback  m_callback=nullptr;

  void  call(menu_event::kind  k) noexcept{if(m_callback){m_callback({*this,k});}}

public:
  menu(                        callback  cb=nullptr) noexcept: m_callback(cb){}
  menu(int  ncols, int  nrows, callback  cb=nullptr) noexcept: m_callback(cb){resize(ncols,nrows);}

  int  get_number_of_columns() const noexcept{return m_number_of_columns;}
  int  get_number_of_rows()    const noexcept{return m_number_of_rows;}

  const item_size&        get_item_size()       const noexcept{return m_item_size;}
  const item_table_size&  get_item_table_size() const noexcept{return m_item_table_size;}

  void  process_before_reform() noexcept override;

  void  revise_cursor() noexcept;

  void  resize(int  ncols, int  nrows) noexcept;

  void  set_item_size(item_size  itm_sz) noexcept;
  void  set_item_table_size(item_table_size  itmtbl_sz) noexcept;

  const item_cursor&  get_item_cursor()  const noexcept{return m_cursor;}

  void  move_up() noexcept;
  void  move_down() noexcept;
  void  move_left() noexcept;
  void  move_right() noexcept;

  void  do_on_mouse_act(point  mouse_pos) noexcept override;

  void  render(const canvas&  cv) noexcept override;

  virtual void  render_background(const canvas&  cv) noexcept{}
  virtual void  render_item(point  item_index, const canvas&  cv) noexcept=0;

};


}


#endif




