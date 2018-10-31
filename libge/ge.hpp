#ifndef gbgui_canvas_hpp_was_included
#define gbgui_canvas_hpp_was_included


#include"libgbstd/windows/component.hpp"
#include"libgbstd/file_op.hpp"


namespace ge{


class
color_maker: public gbstd::widget
{
  gbstd::dial*  m_r_dial;
  gbstd::dial*  m_g_dial;
  gbstd::dial*  m_b_dial;

  gbstd::widget*  m_sample;

  void  (*m_callback)(color_maker&  cm, gbstd::color  new_color)=nullptr;

  gbstd::color  m_color;

  static void  update_color_internal(gbstd::dial_event  evt) noexcept;

public:
  color_maker(void  (*callback)(color_maker&,gbstd::color)) noexcept;

  void                 set_color(gbstd::color  color)       noexcept;
  const gbstd::color&  get_color(                   ) const noexcept{return m_color;}

  void  update_color() noexcept;

};


class
color_holder: public gbstd::widget
{
  int  m_index=0;

  std::vector<gbstd::color>  m_colors;

  void  (*m_callback)(color_holder&  cm, gbstd::color  new_color)=nullptr;

public:
  color_holder(int  size, void  (*callback)(color_holder&,gbstd::color)) noexcept;
  color_holder(std::initializer_list<gbstd::color>  colors, void  (*callback)(color_holder&,gbstd::color)) noexcept;

  int  get_index() const noexcept{return m_index;}

  void           set_color(gbstd::color  color) noexcept;
  gbstd::color&  get_color(                   ) noexcept{return m_colors[m_index];}

  void  render(const gbstd::canvas&  cv) noexcept override;

  void  do_on_mouse_act(gbstd::point  mouse_pos) noexcept override;

};


class core_paint;


class
color_handler
{
  color_holder*  m_holder;
  color_maker*    m_maker;

  core_paint*  m_paint;

  static void  holder_callback(color_holder&  holder, gbstd::color  color) noexcept;
  static void   maker_callback(color_maker&    maker, gbstd::color  color) noexcept;

public:
  color_handler(core_paint&  pai) noexcept;

  const gbstd::color&  get_color() const noexcept{return m_maker->get_color();}

  gbstd::widget*  get_holder() const noexcept{return m_holder;}
  gbstd::widget*   get_maker() const noexcept{return  m_maker;}

};




class context;


class
aniview: public gbstd::widget
{
  int  m_index=0;

  uint32_t  m_delay_time=0;
  uint32_t  m_next_time=0;

  gbstd::label*  m_state_label;

  gbstd::widget*  m_first_child;

  std::vector<gbstd::canvas>  m_frames;
  std::vector<gbstd::point>   m_points;

  static void   psh(gbstd::button_event  evt) noexcept;
  static void   pop(gbstd::button_event  evt) noexcept;
  static void  dial_callback(gbstd::dial_event  evt) noexcept;

public:
  aniview(context&  ctx) noexcept;

  void  clear() noexcept;
  void  check() noexcept;
  void  rebase() noexcept;

  void  process_before_reform() noexcept override;

  void  save_as_apng(const char*  filename) const noexcept;

  void  update_state_label() noexcept;

  void  render(const gbstd::canvas&  cv) noexcept override;

};


class
seamless_pattern_view: public gbstd::widget
{

public:
  seamless_pattern_view(context&  ctx) noexcept;

  void  process_before_reform() noexcept override;

  void  render(const gbstd::canvas&  cv) noexcept override;

};


class
underlay_stacker: public gbstd::widget
{
  gbstd::label*    m_counter_label;
  gbstd::label*  m_switching_label;

  static void   psh(gbstd::button_event  evt) noexcept;
  static void   pop(gbstd::button_event  evt) noexcept;
  static void   swi(gbstd::button_event  evt) noexcept;

public:
  underlay_stacker(context&  ctx) noexcept;

  void  update_counter_label() noexcept;

};



class
drawing_recorder
{
  struct dot{
    gbstd::color  color;
    uint16_t  x;
    uint16_t  y;

    dot(gbstd::color  color_=gbstd::color(), int  x_=0, int  y_=0) noexcept:
    color(color_), x(x_), y(y_){}

  };


  std::vector<dot>  m_dot_buffer;

  struct record;

  record*  m_record_list=nullptr;

  uint32_t  m_number_of_records=0;

public:
  drawing_recorder() noexcept{}
  drawing_recorder(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder(      drawing_recorder&&  rhs) noexcept{*this = std::move(rhs);}
 ~drawing_recorder(){clear();}

  drawing_recorder&  operator=(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder&  operator=(      drawing_recorder&&  rhs) noexcept;

  void  put(gbstd::color  color, int  x, int  y) noexcept{m_dot_buffer.emplace_back(color,x,y);}

  bool  rollback(const gbstd::canvas&  cv) noexcept;

  void  clear() noexcept;

  bool  push(bool  solid) noexcept;

  uint32_t  get_number_of_records() const noexcept{return m_number_of_records;}

};




class
previewer: public gbstd::widget
{
  gbstd::canvas  m_a_base;
  gbstd::canvas  m_b_base;
  gbstd::canvas  m_top_edge;
  gbstd::canvas  m_bottom_edge;
  gbstd::canvas  m_left_edge;
  gbstd::canvas  m_right_edge;

public:
  previewer(const gbstd::image&  img) noexcept;

  void  process_before_reform() noexcept override;

  void  render_product(gbstd::point  pt, int  flags, const gbstd::canvas&  cv) noexcept;

  void  render(const gbstd::canvas&  cv) noexcept override;

};




class core;


class
core_event: public gbstd::event<core>
{
public:
  using event::event;

  enum class kind{
    painting_cursor_moved,
    image_modified,
  };

  bool  is_painting_cursor_moved() const noexcept{return test_kind(kind::painting_cursor_moved);}
  bool  is_image_modified() const noexcept{return test_kind(kind::image_modified);}

};


struct
background_style
{
  gbstd::color  first_color ;
  gbstd::color  second_color;

};


class
core_paint
{
  core*  m_core=nullptr;

  enum class mode{
    draw_dot,
    draw_line,
    draw_rectangle,
    fill_rectangle,
    fill_area,
    select,
    paste,
    layer,

  } m_mode=mode::draw_dot;

  gbstd::color  m_drawing_color=gbstd::color(0,0,0);

  int  m_pointing_count=0;

  gbstd::point        m_drawing_point;
  gbstd::point  m_saved_drawing_point;

  gbstd::rectangle  m_operation_rect;

  gbstd::image  m_clipped_image;

  bool  m_drawing_is_fixed=true;

  void  try_to_push_solid_record()    noexcept;
  void  try_to_push_nonsolid_record() noexcept;

  gbstd::image  get_temporary_image() noexcept;


  void  modify_dot(gbstd::color  color, gbstd::point  pt) noexcept;

  void  draw_line(gbstd::color  color                        ) noexcept;
  void  draw_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept;
  void  fill_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept;
  void  fill_area(gbstd::color  color                        ) noexcept;

  void  feedback() const noexcept;

public:
  void  reset(core&  cor) noexcept;

  void  cancel_drawing() noexcept;
  void  cancel_select() noexcept;

  void  revolve(             ) noexcept;
  void  reverse_horizontally() noexcept;
  void  reverse_vertically(  ) noexcept;
  void  mirror_vertically(   ) noexcept;
  void  shift_up(   bool  rotate) noexcept;
  void  shift_left( bool  rotate) noexcept;
  void  shift_right(bool  rotate) noexcept;
  void  shift_down( bool  rotate) noexcept;

  void  take_copy() noexcept;
  void      paste(bool  layer) noexcept;

  void          set_drawing_point(gbstd::point  pt)       noexcept{       m_drawing_point = pt;}
  gbstd::point  get_drawing_point(                ) const noexcept{return m_drawing_point     ;}

  void          set_drawing_color(gbstd::color  color)       noexcept{       m_drawing_color = color;}
  gbstd::color  get_drawing_color(                   ) const noexcept{return m_drawing_color        ;}

  const gbstd::pixel&  get_current_pixel() const noexcept;

  gbstd::rectangle  get_operation_rectangle() const noexcept{return m_operation_rect;}

  void  change_mode_to_draw_dot()       noexcept{m_mode = mode::draw_dot;}
  void  change_mode_to_draw_line()      noexcept{m_mode = mode::draw_line;}
  void  change_mode_to_draw_rectangle() noexcept{m_mode = mode::draw_rectangle;}
  void  change_mode_to_fill_rectangle() noexcept{m_mode = mode::fill_rectangle;}
  void  change_mode_to_fill_area()      noexcept{m_mode = mode::fill_area;}
  void  change_mode_to_select()         noexcept{m_mode = mode::select;}
  void  change_mode_to_paste()          noexcept{m_mode = mode::paste;}
  void  change_mode_to_layer()          noexcept{m_mode = mode::layer;}

  void  operator()() noexcept;


  gbstd::widget*  make_operation_widget() noexcept;
  gbstd::widget*  make_tool_widget() noexcept;

};


class
core_display
{
  int  m_pixel_size=1;

  struct flags{
    static constexpr int  show_grid       = 1;
    static constexpr int  show_underlay   = 2;
  };


  int  m_state=3;

  std::vector<gbstd::canvas>  m_underlay_stack;
  std::vector<gbstd::point>  m_underlay_points;

  background_style  m_bg_style;

  gbstd::widget*  m_container;

  const gbstd::color*  m_color;

  std::vector<gbstd::widget*>  m_affected_widget_list;

  static void   first_callback(gbstd::button_event  evt) noexcept;
  static void  second_callback(gbstd::button_event  evt) noexcept;

public:
  core_display() noexcept;

  void  reset(const gbstd::color&  color, std::initializer_list<gbstd::widget*>  ls) noexcept;

  gbstd::widget*  get_widget() const noexcept{return m_container;}

  void  push_underlay(gbstd::image&  img, gbstd::point  pt, int  w, int  h) noexcept;
  void   pop_underlay() noexcept;
  int  get_number_of_underlays() const noexcept{return m_underlay_points.size();}

  void   clear_underlay_stack() noexcept;
  void  rebase_underlay_stack(gbstd::image&  img, int  w, int  h) noexcept;

  int   get_pixel_size(      ) const noexcept{return m_pixel_size    ;}
  void  set_pixel_size(int  n)       noexcept{       m_pixel_size = n;}

  void                     set_background_style(background_style  bgst)       noexcept{       m_bg_style = bgst;}
  const background_style&  get_background_style(                      ) const noexcept{return m_bg_style       ;}

  void      show_grid() noexcept{m_state |=  flags::show_grid;}
  void      hide_grid() noexcept{m_state &= ~flags::show_grid;}
  void  show_underlay() noexcept{m_state |=  flags::show_underlay;}
  void  hide_underlay() noexcept{m_state &= ~flags::show_underlay;}

  bool  test_whether_show_grid()     const noexcept{return m_state&flags::show_grid;}
  bool  test_whether_show_underlay() const noexcept{return m_state&flags::show_underlay;}

  void  render_rect(gbstd::rectangle  rect, const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;

  void  render_grid(      const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;
  void  render_underlay(  const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;
  void  render_background(const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;
  void  render_canvas(const gbstd::canvas&  src_cv, const gbstd::canvas&  dst_cv, int  pixel_size=0) const noexcept;

};


class
core_view: public gbstd::widget
{
protected:
  gbstd::canvas  m_canvas;

  core_display*  m_display=nullptr;

public:
  core_view(core_display&  dsp) noexcept;

  core_display&  get_display() const noexcept{return *m_display;}

  const gbstd::canvas&  get_canvas(                        ) const noexcept{return m_canvas     ;}
  virtual void          set_canvas(const gbstd::canvas&  cv)       noexcept{       m_canvas = cv;}

  void  process_before_reform() noexcept override;

  void  render(const gbstd::canvas&  cv) noexcept override;

};


class
core: public core_view
{
  core_paint*  m_paint=nullptr;

  drawing_recorder  m_recorder;

  std::vector<gbstd::widget*>  m_affected_widget_list;

  void  (*m_callback)(core_event  evt)=nullptr;

  bool  m_focus=false;

public:
  core(core_paint&  pai, core_display&  dsp, void  (*callback)(core_event  evt)=nullptr) noexcept;

  void  set_affected_widget_list(std::initializer_list<gbstd::widget*>  ls) noexcept{m_affected_widget_list = ls;}

  void  request_redraw_all() noexcept;

  drawing_recorder&  get_recorder() noexcept{return m_recorder;}

  core_paint&  get_paint() const noexcept{return *m_paint;}

  void  set_canvas(const gbstd::canvas&  cv) noexcept override;

  void  revolve()                 noexcept{m_paint->revolve();}
  void  reverse_horizontally()    noexcept{m_paint->reverse_horizontally();}
  void  reverse_vertically()      noexcept{m_paint->reverse_vertically();}
  void  mirror_vertically()       noexcept{m_paint->mirror_vertically();}
  void  shift_up(bool  rotate)    noexcept{m_paint->shift_up(rotate);}
  void  shift_left(bool  rotate)  noexcept{m_paint->shift_left(rotate);}
  void  shift_right(bool  rotate) noexcept{m_paint->shift_right(rotate);}
  void  shift_down(bool  rotate)  noexcept{m_paint->shift_down(rotate);}

  void  take_copy() noexcept{m_paint->take_copy();}
  void       undo() noexcept;
  void      paste(bool  layer) noexcept{m_paint->paste(layer);}


  void  do_on_mouse_enter() noexcept override;
  void  do_on_mouse_leave() noexcept override;

  void  do_on_mouse_act(gbstd::point  mouse_pos) noexcept override;

  void  render(const gbstd::canvas&  cv) noexcept override;

};


}


#endif




