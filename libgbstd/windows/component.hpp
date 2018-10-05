#ifndef gbstd_window_component_hpp_was_included
#define gbstd_window_component_hpp_was_included


#include"libgbstd/window.hpp"


namespace gbstd{




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
  frame(const char*  text) noexcept;

  const char*  get_class_name() const noexcept override{return "frame";}

  void  append_content(widget*  wg, int  x, int  y) noexcept;

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
button_event
{
  button*  m_button;

  int  m_data;

public:
  constexpr button_event(button&  btn,  int  dat) noexcept: m_button(&btn), m_data(dat){}

  constexpr button*  operator->() const noexcept{return  m_button;}
  constexpr button&  operator*()  const noexcept{return *m_button;}

  constexpr bool  is_press()   const noexcept{return m_data == 1;}
  constexpr bool  is_release() const noexcept{return m_data == 0;}

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

};




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
  widget(g_icon_size,g_icon_size), m_icons(ls), m_callback(cb){}

  const icon*  operator*() const noexcept{return m_icons[m_index];}

  int   get_index(      ) const noexcept{return m_index;}
  void  set_index(int  i)       noexcept;

  const char*  get_class_name() const noexcept override{return "iconshow";}

  void  do_on_mouse_enter() noexcept override{if(m_callback){m_callback({*this,iconshow_event::kind::enter});}}
  void  do_on_mouse_leave() noexcept override;
  void  do_on_mouse_act(point  mouse_pos) noexcept override;

  void  render(const canvas&  cv) noexcept override;

};




class checkbox;


class
checkbox_event
{
  checkbox*  m_checkbox;

  int  m_kind;

public:
  checkbox_event(checkbox&  chkbox, int  k) noexcept: m_checkbox(&chkbox), m_kind(k){}

  checkbox*  operator->() const noexcept{return m_checkbox;}

  bool  is_change_to_set()   const noexcept{return m_kind == 0;}
  bool  is_change_to_unset() const noexcept{return m_kind == 1;}

};




class
checkbox: public widget
{
  struct shared_data;

  shared_data*  m_data;

  iconshow*  m_iconshow;
  label*     m_label;

  uint32_t  m_bit_id;

  checkbox*  m_next=nullptr;

  static void  iconshow_callback(iconshow_event  evt) noexcept;

public:
  checkbox(                         const char16_t*  text) noexcept;
  checkbox(const checkbox&  member, const char16_t*  text) noexcept;
 ~checkbox();

  const char*  get_class_name() const noexcept override{return "checkbox";}

  void  do_on_mouse_act(point  mouse_pos) noexcept;

  uint32_t  get_bit_id() const noexcept{return m_bit_id;}

};




/*
class
radio_button: public widget
{
public:
  using callback_prototype = void  (*)(radio_button&  btn, uint32_t  old_state, uint32_t  new_state);

private:
  struct shared_data;

  shared_data*  m_data;

  radio_button*  m_next=nullptr;

protected:
  uint32_t  m_bit_id;

  icon_selector*  m_icons;

  virtual const icon**  get_icons() const noexcept;

  void  call(uint32_t  new_state) noexcept;

public:
  radio_button(widget*  target, callback_prototype  cb) noexcept;
  radio_button(widget*  target, radio_button&  first) noexcept;
 ~radio_button();

  const char*  get_class_name() const noexcept override{return "radio_button";}

  void  update() noexcept override;

  bool  is_checked() const noexcept;

  virtual void  change_state() noexcept;

  uint32_t  get_bit_id() const noexcept{return m_bit_id;}

  uint32_t  get_state(            ) const noexcept;
  void      set_state(uint32_t  st)       noexcept;

};




class
dial: public table_row
{
  int  m_current=0;

  int  m_min=0;
  int  m_max=0;

  void  (*m_callback)(dial&  dial, int  old_value, int  new_value)=nullptr;

  icon_selector*     m_up_selector;
  icon_selector*   m_down_selector;

  button*     m_up_button;
  button*   m_down_button;

  label*   m_label;

  static void    up(button&  btn);
  static void  down(button&  btn);

  void  update_label() noexcept;

public:
  dial(int  min, int  max, void  (*callback)(dial&,int,int)) noexcept;

  void  set_current(int  v) noexcept;

  int  get_current() const noexcept{return m_current;}
  int  get_min() const noexcept{return m_min;}
  int  get_max() const noexcept{return m_max;}

};


using rbcb = radio_button::callback_prototype;
using wls = std::initializer_list<widget*>;

widget*  create_radio_menu(wls  ls, rbcb  cb, uint32_t  initial_state=0, void*  userdata=nullptr) noexcept;
widget*  create_check_menu(wls  ls, rbcb  cb, uint32_t  initial_state=0, void*  userdata=nullptr) noexcept;
*/



}


#endif




