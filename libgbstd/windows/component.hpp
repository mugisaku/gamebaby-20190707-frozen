#ifndef gbstd_window_component_hpp_was_included
#define gbstd_window_component_hpp_was_included


#include"libgbstd/window.hpp"


namespace gbstd{




/*
class
frame: public widget
{
  std::u16string  m_text;

  character_color  m_character_color;

  color  m_line_color;

  static color  m_default_line_color;

public:
  frame(widget*  target, const char*  text, const character_color&  color) noexcept:
  m_text(text), m_character_color(cooor),
  m_line_color(m_default_line_color)
  {}

  const char*  get_class_name() const noexcept override{return "frame";}

  void  reform(point  base_pt) noexcept override;

  void  set_line_color(color  new_color) noexcept;

  void  render(image_cursor  cur) noexcept override;

  static void  set_default_line_color(color  new_color) noexcept{m_default_line_color = new_color;}

};
*/




class
label: public widget
{
  static character_color  m_default_color;

  character_color  m_color;

  std::u16string  m_text;

public:
  label(const char*      s                               ) noexcept;
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


/*
class
button: public widget
{
  std::unique_ptr<widget>  m_target;

  void  (*m_callback)(button&  button)=nullptr;

  enum class state{
    released,
     pressed,
  } m_state=state::released;


  int  m_count=0;

public:
  button(widget*  target, void  (*callback)(button&)) noexcept;

  const char*  get_class_name() const noexcept override{return "button";}

  int  get_count() const noexcept{return m_count;}

  void  reset_count() noexcept{m_count = 0;}

  bool  is_pressed()  const noexcept{return m_state ==  state::pressed;}
  bool  is_released() const noexcept{return m_state == state::released;}

  void  reform(point  base_pt) noexcept override;

  void  do_when_cursor_got_out() noexcept override;

  void  update() noexcept override;

  void  render(image_cursor  cur) noexcept override;

  void  print(int  indent=0) const noexcept override;

  void  show_all() noexcept override;

};


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
check_button: public radio_button
{
  const icon**  get_icons() const noexcept override;

public:
  using radio_button::radio_button;

  const char*  get_class_name() const noexcept override{return "check_button";}

  void  change_state() noexcept override;

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




