#ifndef gbstd_widgets_hpp_was_included
#define gbstd_widgets_hpp_was_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/menu.hpp"
#include<memory>
#include<cstdio>


namespace gbstd{
namespace widgets{




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




class
style
{
  color  m_background_color=colors::light_gray;

  int  m_left_padding  =0;
  int  m_top_padding   =0;
  int  m_right_padding =0;
  int  m_bottom_padding=0;

public:
  style() noexcept{}

  color  get_background_color()    const noexcept{return m_background_color;}

  int  get_top_padding()    const noexcept{return m_top_padding   ;}
  int  get_left_padding()   const noexcept{return m_left_padding  ;}
  int  get_right_padding()  const noexcept{return m_right_padding ;}
  int  get_bottom_padding() const noexcept{return m_bottom_padding;}

  style&  set_background_color(color  c) noexcept{  m_background_color = c;  return *this;}
  style&  set_top_padding(   int  v) noexcept{  m_top_padding    = v;  return *this;}
  style&  set_left_padding(  int  v) noexcept{  m_left_padding   = v;  return *this;}
  style&  set_right_padding( int  v) noexcept{  m_right_padding  = v;  return *this;}
  style&  set_bottom_padding(int  v) noexcept{  m_bottom_padding = v;  return *this;}

  void  print() const noexcept{
    printf("top-padding:%d,\nleft-padding:%d,\nright-padding:%d,\nbottom-padding:%d,\n",m_top_padding,m_left_padding,m_right_padding,m_bottom_padding);
  }

};


class operating_node;


class
node
{
  friend class operating_node;

  point  m_relative_position;
  point  m_absolute_position;

  int  m_content_width =0;
  int  m_content_height=0;

  int  m_minimal_content_width =0;
  int  m_minimal_content_height=0;


  std::string  m_id;
  std::string  m_name;

  node*  m_parent=nullptr;

  node*  m_first_child=nullptr;
  node*  m_last_child=nullptr;

  int  m_number_of_children=0;

  node*  m_previous_sibling=nullptr;
  node*  m_next_sibling=nullptr;

  void*  m_userdata=nullptr;

protected:
  enum class follow_style{
    none,
    right,
    bottom,

  } m_follow_style=follow_style::none;


  struct flags{
    static constexpr int         hidden =  2;
    static constexpr int      displayed =  4;
    static constexpr int         frozen =  8;
    static constexpr int         queued = 16;
    static constexpr int     autonomous = 32;

  };


  style  m_style;

  status_value<int>  m_status;

  virtual void  relay_redraw_request(node&  nd) noexcept{if(m_parent){m_parent->relay_redraw_request(nd);}}
  virtual void  relay_reform_request(         ) noexcept{if(m_parent){m_parent->relay_reform_request(  );}}

  virtual void  process_before_reform() noexcept{}

  node() noexcept{m_status.set(flags::displayed);}
  node(const node&   rhs) noexcept=delete;
  node(      node&&  rhs) noexcept=delete;

  node&  operator=(const node&   rhs) noexcept=delete;
  node&  operator=(      node&&  rhs) noexcept=delete;

 virtual ~node(){clear();}

public:
  class iterator{
    node*  m_pointer=nullptr;

  public:
    iterator() noexcept{}
    iterator(node*  ptr) noexcept: m_pointer(ptr){}

    operator bool() const noexcept{return m_pointer;}

    node&   operator*() const noexcept{return *m_pointer;}
    node*  operator->() const noexcept{return  m_pointer;}

    bool  operator==(iterator  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
    bool  operator!=(iterator  rhs) const noexcept{return m_pointer != rhs.m_pointer;}

    iterator&  operator++() noexcept{  m_pointer = m_pointer->m_next_sibling;  return *this;}
    iterator&  operator--() noexcept{  m_pointer = m_pointer->m_next_sibling;  return *this;}

    iterator  operator++(int) noexcept;
    iterator  operator--(int) noexcept;

    iterator&  operator+=(int  n) noexcept;
    iterator&  operator-=(int  n) noexcept;

  };

  iterator  begin() const noexcept{return iterator(m_first_child);}
  iterator    end() const noexcept{return iterator(nullptr);}


  node&  clear() noexcept;

  virtual std::string_view  get_class_name() const noexcept{return "node";}

  std::string_view    get_id() const noexcept{return m_id;}
  std::string_view  get_name() const noexcept{return m_name;}

  status_value<int>&  get_status() noexcept{return m_status;}

  const style&  get_style() const noexcept{return m_style;}


  node&  add_child(node&  child, point  pt) noexcept;

  node&  add_child_as_row(   std::initializer_list<std::reference_wrapper<node>>  children) noexcept;
  node&  add_child_as_column(std::initializer_list<std::reference_wrapper<node>>  children) noexcept;

  int  get_number_of_children() const noexcept{return m_number_of_children;}


  point  get_relative_position() const noexcept{return m_relative_position;}
  point  get_absolute_position() const noexcept{return m_absolute_position;}

  node&  set_relative_position(int  x, int  y) noexcept{m_relative_position  = point(x,y);  return *this;}
  node&  set_relative_position(point  pt)      noexcept{m_relative_position  = pt;  return *this;}
  node&  add_relative_position(int  x, int  y) noexcept{m_relative_position += point(x,y);  return *this;}
  node&  add_relative_position(point  pt)      noexcept{m_relative_position += pt;  return *this;}

  int  get_content_top()  const noexcept{return m_absolute_position.y+m_style.get_top_padding();}
  int  get_content_left() const noexcept{return m_absolute_position.x+m_style.get_left_padding();}

  point  get_content_position() const noexcept{return point(get_content_left(),get_content_top());}

  int  get_content_width()  const noexcept{return m_content_width ;}
  int  get_content_height() const noexcept{return m_content_height;}

  int  get_minimal_content_width()  const noexcept{return m_minimal_content_width ;}
  int  get_minimal_content_height() const noexcept{return m_minimal_content_height;}

  rectangle  get_content_rectangle() const noexcept;

  node&  set_content_width( int  v) noexcept{  m_content_width  = v;  return *this;}
  node&  set_content_height(int  v) noexcept{  m_content_height = v;  return *this;}

  node&  set_minimal_content_width( int  v) noexcept{  m_minimal_content_width  = v;  return *this;}
  node&  set_minimal_content_height(int  v) noexcept{  m_minimal_content_height = v;  return *this;}

  int  get_width()  const noexcept{return m_style.get_left_padding()+m_content_width +m_style.get_right_padding();}
  int  get_height() const noexcept{return m_style.get_top_padding() +m_content_height+m_style.get_bottom_padding();}


  bool  is_frozen()     const noexcept{return m_status.test(flags::frozen);}
  bool  is_hidden()     const noexcept{return m_status.test(flags::hidden);}
  bool  is_displayed()  const noexcept{return m_status.test(flags::displayed);}
  bool  is_autonomous() const noexcept{return m_status.test(flags::autonomous);}

  node&  be_autonomous() noexcept{  m_status.set(flags::autonomous);  return *this;}

  node&    freeze() noexcept{    m_status.set(flags::frozen);  return *this;}
  node&  unfreeze() noexcept{  m_status.unset(flags::frozen);  return *this;}

  node&    display() noexcept{    m_status.set(flags::displayed);  return *this;}
  node&  undisplay() noexcept{  m_status.unset(flags::displayed);  return *this;}

  node&  show() noexcept{  m_status.unset(flags::hidden);  return *this;}
  node&  hide() noexcept{    m_status.set(flags::hidden);  return *this;}


  bool      test_by_point(point  pt) const noexcept;
  iterator  get_node_by_point(point  pt) noexcept;

  iterator  get_node_by_id(std::string_view  id) const noexcept;

  node&  get_nodes_by_name(      std::string_view  name, std::vector<iterator>&  ls) noexcept;
  node&  get_nodes_by_class_name(std::string_view  name, std::vector<iterator>&  ls) noexcept;

  std::vector<iterator>  get_nodes_by_name(      std::string_view  name) noexcept;
  std::vector<iterator>  get_nodes_by_class_name(std::string_view  name) noexcept;


  virtual void  on_mouse_enter() noexcept{}
  virtual void  on_mouse_leave() noexcept{}
  virtual void  on_mouse_act(point  mouse_pos) noexcept{}

  virtual void  render(const canvas&  cv) noexcept{}

  void  reform(point  parent_content_position) noexcept;
  void  redraw(const canvas&  cv) noexcept;

  void  request_redraw() noexcept{if(m_parent && !m_status.test(flags::queued)){m_parent->relay_redraw_request(*this);}}
  void  request_reform() noexcept{if(m_parent                                 ){m_parent->relay_reform_request(     );}}

  void  fill(const canvas&  cv, gbstd::color  color) const noexcept;
  void  fill(const canvas&  cv, color  color0, color  color1, int  interval) const noexcept;

  node&                    set_userdata(void*   ptr)       noexcept{  m_userdata = ptr;  return *this;}
  template<typename  T>T&  get_userdata(           ) const noexcept{return *static_cast<T*>(m_userdata);}

};




class label;


class
frame: public node
{
  friend class operating_node;

  static color  m_default_color;
  static color  m_default_line_color;

  std::u16string  m_string;

  color  m_color=m_default_color;
  color  m_line_color=m_default_line_color;

  frame(operating_node&  root) noexcept;

public:
  std::string_view  get_class_name() const noexcept override{return "frame";}

  frame&  set_content(node&  nd) noexcept;

  frame&  set_string(std::string_view     sv) noexcept;
  frame&  set_string(std::u16string_view  sv) noexcept;
  frame&  set_line_color(color  new_color) noexcept;

  void  render(const canvas&  cv) noexcept override;

  static void  set_default_line_color(color  new_color) noexcept{m_default_line_color = new_color;}

};


class
label: public node
{
  friend class operating_node;

  static color  m_default_color;

  color  m_color=m_default_color;

  std::u16string  m_string;

  label(operating_node&  root) noexcept;

public:
  std::string_view  get_class_name() const noexcept override{return "label";}

  static void          set_default_color(color  col) noexcept{       m_default_color = col;}
  static const color&  get_default_color(          ) noexcept{return m_default_color      ;}

  const std::u16string&  get_string() const noexcept{return m_string;}

  label&  set_string(std::string_view     sv) noexcept;
  label&  set_string(std::u16string_view  sv) noexcept;

  label&  set_string_without_reform(std::string_view     sv) noexcept;
  label&  set_string_without_reform(std::u16string_view  sv) noexcept;

  label&  set_color(color  new_color) noexcept;

  void  render(const canvas&  cv) noexcept override;

};




template<typename  T>
class
event
{
  T*  m_caller;

  int  m_value;

public:
                       constexpr event(T&  caller      ) noexcept: m_caller(&caller), m_value(0){}
  template<typename  E>constexpr event(T&  caller, E  e) noexcept: m_caller(&caller), m_value(static_cast<int>(e)){}

  constexpr T*  operator->() const noexcept{return  m_caller;}
  constexpr T&  operator*()  const noexcept{return *m_caller;}

  constexpr int  get_value() const noexcept{return m_value;}

  template<typename  E>constexpr bool  operator==(E  e) const noexcept{return m_value == static_cast<int>(e);}

};




class button;


class
button_event: public event<button>
{
public:
  using event::event;

  enum class kind{
      press,
    release,
  };

  constexpr bool  is_press()   const noexcept{return *this == kind::press  ;}
  constexpr bool  is_release() const noexcept{return *this == kind::release;}

};


class
button: public node
{
  friend class operating_node;

  void  (*m_callback)(button_event)=nullptr;

  enum class state{
    released,
     pressed,
  } m_state=state::released;


  button(operating_node&  root) noexcept;

public:
  button&  set_content(node&  nd) noexcept{  clear();  add_child(nd,{});  return *this;}

  button&  set_callback(void  (*cb)(button_event)) noexcept{  m_callback = cb;  return *this;}

  std::string_view  get_class_name() const noexcept override{return "button";}

  bool  is_pressed()  const noexcept{return m_state ==  state::pressed;}
  bool  is_released() const noexcept{return m_state == state::released;}

  void  on_mouse_leave() noexcept override;
  void  on_mouse_act(point  mouse_pos) noexcept override;

  void  render(const canvas&  cv) noexcept override;

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

  bool  is_enter() const noexcept{return *this == kind::enter;}
  bool  is_leave() const noexcept{return *this == kind::leave;}
  bool  is_click() const noexcept{return *this == kind::click;}

};


class
iconshow: public node
{
  friend class operating_node;

  std::vector<const icon*>  m_icons;

  int  m_index=0;

  bool  m_whether_touched=false;

  void  (*m_callback)(iconshow_event  evt)=nullptr;

  iconshow(operating_node&  root) noexcept;

public:
  const icon*  operator*() const noexcept{return m_icons[m_index];}

  int        get_index(      ) const noexcept{return m_index;}
  iconshow&  set_index(int  i)       noexcept;

  iconshow&  set_icon_list(std::initializer_list<const icon*>  ls) noexcept;
  iconshow&  set_callback(void  (*cb)(iconshow_event)) noexcept;

  std::string_view  get_class_name() const noexcept override{return "iconshow";}

  void  on_mouse_enter() noexcept override{if(m_callback){m_callback({*this,iconshow_event::kind::enter});}}
  void  on_mouse_leave() noexcept override;
  void  on_mouse_act(point  mouse_pos) noexcept override;

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

  bool  is_set()   const noexcept{return *this == kind::set;}
  bool  is_unset() const noexcept{return *this == kind::unset;}

};




class
checkbox: public node
{
  friend class operating_node;

  struct shared_data;

  shared_data*  m_data;

  std::reference_wrapper<iconshow>  m_iconshow;
  std::reference_wrapper<label>     m_label;

  uint32_t  m_entry_number=0;

  checkbox*  m_next=nullptr;

  void*  get_common_userdata_internal() const noexcept;

protected:
  void  revise_to_radio() noexcept;

  checkbox(operating_node&  root                         ) noexcept;
  checkbox(operating_node&  root, const checkbox&  member) noexcept;

public:
  ~checkbox();

  checkbox&  set_string(std::string_view     sv) noexcept;
  checkbox&  set_string(std::u16string_view  sv) noexcept;
  checkbox&  set_callback(void  (*cb)(checkbox_event)) noexcept;

  void    check() noexcept;
  void  uncheck() noexcept;

  std::string_view  get_class_name() const noexcept override{return "checkbox";}

  bool  is_checked() const noexcept{return m_iconshow.get().get_index();}

  operator bool() const noexcept{return is_checked();}

  uint32_t  get_entry_number() const noexcept{return m_entry_number;}

  void  set_common_userdata(void*  ptr) const noexcept;

  template<typename  T>
  T*  get_common_userdata() const noexcept{return static_cast<T*>(get_common_userdata_internal());}

  void  on_mouse_act(point  mouse_pos) noexcept override;

};




class
radio_button: public checkbox
{
  friend class operating_node;

  radio_button(operating_node&  root                         ) noexcept;
  radio_button(operating_node&  root, const checkbox&  member) noexcept;

public:
  std::string_view  get_class_name() const noexcept override{return "radio_button";}

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
dial: public node
{
  friend class operating_node;

  int  m_current=0;

  int  m_min=0;
  int  m_max=0;

  void  (*m_callback)(dial_event  evt)=nullptr;

  std::reference_wrapper<iconshow>     m_up_show;
  std::reference_wrapper<iconshow>   m_down_show;

  std::reference_wrapper<button>     m_up_button;
  std::reference_wrapper<button>   m_down_button;

  std::reference_wrapper<label>   m_label;

  static void    up(button_event  evt) noexcept;
  static void  down(button_event  evt) noexcept;

  void  update_label() noexcept;

  dial(operating_node&  root) noexcept;

public:
  dial&  set_callback(void  (*cb)(dial_event)) noexcept{  m_callback = cb;  return *this;}

  dial&  set_max(int  v) noexcept;
  dial&  set_min(int  v) noexcept;

  dial&  set_current(int  v) noexcept;

  int  get_current() const noexcept{return m_current;}

  int  get_min() const noexcept{return m_min;}
  int  get_max() const noexcept{return m_max;}

};




class menu_handler;


class
menu_event: public event<menu_handler>
{
public:
  using event::event;

  enum class kind{
    enter,
    leave,
    press,
    release,
  };

  bool  is_enter() const noexcept{return *this == kind::enter;}
  bool  is_leave() const noexcept{return *this == kind::leave;}
  bool  is_press() const noexcept{return *this == kind::press;}
  bool  is_release() const noexcept{return *this == kind::release;}

};


class
menu_handler: public node
{
  friend class operating_node;

public:
  using callback = void(*)(menu_event  evt);

private:
  menus::view*  m_view;

  callback  m_callback=nullptr;

  void  call(menu_event::kind  k) noexcept{if(m_callback){m_callback({*this,k});}}

  void  process_before_reform() noexcept override;

  menu_handler(operating_node&  root) noexcept;

public:
  menus::view&  get_view() const noexcept{return *m_view;}

  menu_handler&  set_view(menus::view&  v) noexcept;
  menu_handler&  set_callback(callback  cb=nullptr) noexcept{  m_callback = cb;  return *this;}

  void  on_mouse_act(point  mouse_pos) noexcept override;

  void  render(const canvas&  cv) noexcept override;

};




class
operating_node: public node
{
  image  m_image;

  node::iterator  m_current=nullptr;

  std::vector<node*>  m_queue;

  int  m_reform_status=1;

  void  relay_redraw_request(node&  nd) noexcept override;
  void  relay_reform_request(         ) noexcept override;

  void  process_by_mouse_position(point&  pt) noexcept;

public:
  operating_node() noexcept;

  const image&  get_image() const noexcept{return m_image;}

  std::string_view  get_class_name() const noexcept override{return "operating node";}

  void  reform_if_necessary() noexcept;
  bool  redraw_if_necessary() noexcept;

  void  process_user_input(point  pt) noexcept;


  node&          create_node() noexcept{return *(new     node(     ));}
  label&        create_label() noexcept{return *(new    label(*this));}
  button&      create_button() noexcept{return *(new   button(*this));}
  frame&        create_frame() noexcept{return *(new    frame(*this));}
  iconshow&  create_iconshow() noexcept{return *(new iconshow(*this));}
  dial&          create_dial() noexcept{return *(new     dial(*this));}

  checkbox&  create_checkbox(                 ) noexcept{return *(new checkbox(*this       ));}
  checkbox&  create_checkbox(checkbox&  member) noexcept{return *(new checkbox(*this,member));}

  radio_button&  create_radio_button(                 ) noexcept{return *(new radio_button(*this       ));}
  radio_button&  create_radio_button(checkbox&  member) noexcept{return *(new radio_button(*this,member));}

  menu_handler&  create_menu_handler() noexcept{return *(new menu_handler(*this));}

  node&  create_table_column(std::initializer_list<std::reference_wrapper<node>>  ls) noexcept;
  node&  create_table_row(   std::initializer_list<std::reference_wrapper<node>>  ls) noexcept;

};



}}




#endif




