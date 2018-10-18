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

  void          set_color(gbstd::color  color)       noexcept;
  gbstd::color  get_color(                   ) const noexcept{return m_color;}

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
core: public gbstd::widget
{
  gbstd::canvas  m_canvas;

  int  m_pixel_size=1;

  struct flags{
    static constexpr int  show_grid       = 1;
    static constexpr int  show_underlay   = 2;
  };


  int  m_state=3;

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


  drawing_recorder  m_recorder;

  bool  m_drawing_is_fixed=true;

  gbstd::color  m_drawing_color=gbstd::color(0,0,0);

  gbstd::image  m_clipped_image;

  std::vector<gbstd::canvas>  m_underlay_stack;
  std::vector<gbstd::point>  m_underlay_points;

  int  m_pointing_count=0;

  gbstd::point  m_current_point;

  gbstd::point  m_a_point;
  gbstd::point  m_b_point;

  gbstd::rectangle  m_operation_rect;

  background_style  m_bg_style;

  void  (*m_callback)(core_event  evt)=nullptr;

  void  try_to_push_solid_record()    noexcept;
  void  try_to_push_nonsolid_record() noexcept;

  gbstd::image  get_temporary_image() const noexcept;

public:
  core(void  (*callback)(core_event  evt)=nullptr) noexcept;

  void  push_underlay() noexcept;
  void   pop_underlay() noexcept;
  int  get_number_of_underlays() const noexcept{return m_underlay_points.size();}

  void   clear_underlay_stack() noexcept;
  void  rebase_underlay_stack() noexcept;

  const gbstd::canvas&     get_canvas() const noexcept{return m_canvas;}
  void                  rebase_canvas()       noexcept;

  int   get_pixel_size(      ) const noexcept{return m_pixel_size;}
  void  set_pixel_size(int  n)       noexcept;

  void  reset() noexcept;

  const gbstd::point&  get_current_point() const noexcept{return m_current_point;}

  void          set_drawing_color(gbstd::color  color)       noexcept{       m_drawing_color = color;}
  gbstd::color  get_drawing_color(                   ) const noexcept{return m_drawing_color        ;}

  void                     set_background_style(background_style  bgst)       noexcept;
  const background_style&  get_background_style(                      ) const noexcept{return m_bg_style;}

  drawing_recorder&  get_drawing_recorder() noexcept{return m_recorder;}


  void      show_grid() noexcept;
  void      hide_grid() noexcept;
  void  show_underlay() noexcept;
  void  hide_underlay() noexcept;

  bool  test_whether_show_grid()       const noexcept{return m_state&flags::show_grid;}
  bool  test_whether_show_underlay()   const noexcept{return m_state&flags::show_underlay;}


  void  change_mode_to_draw_dot()       noexcept{m_mode = mode::draw_dot;}
  void  change_mode_to_draw_line()      noexcept{m_mode = mode::draw_line;}
  void  change_mode_to_draw_rectangle() noexcept{m_mode = mode::draw_rectangle;}
  void  change_mode_to_fill_rectangle() noexcept{m_mode = mode::fill_rectangle;}
  void  change_mode_to_fill_area()      noexcept{m_mode = mode::fill_area;}
  void  change_mode_to_select()         noexcept{m_mode = mode::select;}
  void  change_mode_to_paste()          noexcept{m_mode = mode::paste;}
  void  change_mode_to_layer()          noexcept{m_mode = mode::layer;}

  void  set_image(const gbstd::image&  img, int  w, int  h) noexcept;

  const gbstd::pixel&  get_pixel(int  x, int  y) const noexcept{return *m_canvas.get_pixel_pointer(x,y);}

  void  modify_dot(gbstd::color  new_color, int  x, int  y) noexcept;

  void  revolve() noexcept;
  void  reverse_horizontally() noexcept;
  void  reverse_vertically() noexcept;
  void  mirror_vertically() noexcept;
  void  shift_up(bool  rotate) noexcept;
  void  shift_left(bool  rotate) noexcept;
  void  shift_right(bool  rotate) noexcept;
  void  shift_down(bool  rotate) noexcept;

  void  draw_line(gbstd::color  color, gbstd::point  a, gbstd::point  b) noexcept;
  void  draw_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept;
  void  fill_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept;
  void  fill_area(gbstd::color  color, gbstd::point  pt) noexcept;

  gbstd::widget*  create_tool_widget() noexcept;
  gbstd::widget*  create_operation_widget() noexcept;

  void  cancel_drawing() noexcept;
  void  cancel_select() noexcept;

  void  take_copy() noexcept;
  void  paste(gbstd::point  pt, bool  layer) noexcept;
  void  undo() noexcept;

  void  do_on_mouse_leave() noexcept override;

  void  do_on_mouse_act(gbstd::point  mouse_pos) noexcept override;

  void  process_before_reform() noexcept override;

  void  render(const gbstd::canvas&  cv) noexcept override;

  void  render_underlay(  int  pixel_size, const gbstd::canvas&  cv) const noexcept;
  void  render_background(int  pixel_size, const gbstd::canvas&  cv) const noexcept;

};


struct
context
{
  gbstd::image  m_source_image;

  gbstd::point  m_current_index;
  gbstd::point  m_seeking_index;


  core*  m_core;

  gbstd::frame*   m_core_frame;

  gbstd::frame*   m_cell_table_frame;

  color_maker*   m_color_maker;
  color_holder*  m_color_holder;

  gbstd::frame*  m_color_maker_frame;
  gbstd::frame*  m_color_holder_frame;

  gbstd::frame*  m_tool_widget_frame;
  gbstd::frame*  m_operation_widget_frame;

  gbstd::widget*  m_bg_change_buttons;

  gbstd::label*  m_cursor_label;
  gbstd::label*  m_table_offset_label;

  gbstd::button*   m_png_save_button;
  gbstd::button*  m_apng_save_button;

  gbstd::menu*  m_menu;


  aniview*  m_aniview=nullptr;
  gbstd::frame*  m_aniview_frame;

  underlay_stacker*  m_underlay_stacker=nullptr;
  gbstd::frame*      m_underlay_stacker_frame;

  std::string  m_filepath;


  gbstd::rectangle  get_rect(gbstd::point  index) const noexcept;

  void  load(const std::vector<uint8_t>&  bin) noexcept;

  context(gbstd::item_size  cell_size, gbstd::item_table_size  table_size) noexcept;

  void  build_core() noexcept;
  void  build_bgcolor_changer() noexcept;
  void  build_color_handler() noexcept;
  void  build_cell_table() noexcept;
  void  build_animation() noexcept;
  void  build_underlay() noexcept;

  void  update_table_offset_label() noexcept;

  void  resize_cell(gbstd::item_size  cell_size) noexcept;
  void  resize_table(gbstd::item_table_size  table_size) noexcept;
  void  resize_image(gbstd::item_size  cell_size, gbstd::item_table_size  table_size) noexcept;

  void  revise() noexcept;

  int  get_cell_width()  const noexcept{return m_menu->get_item_size().width ;}
  int  get_cell_height() const noexcept{return m_menu->get_item_size().height;}

  int  get_table_width()  const noexcept{return m_menu->get_item_table_size().x_length;}
  int  get_table_height() const noexcept{return m_menu->get_item_table_size().y_length;}

};


}


#endif




