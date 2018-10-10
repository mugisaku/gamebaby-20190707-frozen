#ifndef gbgui_canvas_hpp_was_included
#define gbgui_canvas_hpp_was_included


#include"libgbstd/windows/component.hpp"




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


class
core: public gbstd::widget
{
  const gbstd::image*  m_image=nullptr;

  gbstd::canvas  m_canvas;

  gbstd::point  m_cursor_offset;

  int  m_editing_width ;
  int  m_editing_height;

  int  m_pixel_size=1;

  bool  m_grid=false;

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

  const std::vector<gbstd::point>*  m_underlay_point_list=nullptr;

  int  m_pointing_count=0;

  gbstd::point  m_current_point;

  gbstd::point  m_a_point;
  gbstd::point  m_b_point;

  gbstd::rectangle  m_operation_rect;

  void  (*m_callback)(core_event  evt)=nullptr;

  void  try_to_push_solid_record()    noexcept;
  void  try_to_push_nonsolid_record() noexcept;

  gbstd::image  get_temporary_image() const noexcept;

public:
  core() noexcept;
  core(const gbstd::image&  img, int  w, int  h, void  (*callback)(core_event  evt)) noexcept;

  const std::vector<gbstd::point>*  get_underlay_point_list(                                    ) const noexcept{return m_underlay_point_list     ;}
  void                              set_underlay_point_list(const std::vector<gbstd::point>*  ls)       noexcept{       m_underlay_point_list = ls;}

  gbstd::point  get_cursor_offset(              ) const noexcept{return m_cursor_offset;}
  void          set_cursor_offset(int  x, int  y) noexcept;

  void  set_editing_size(int  w, int  h) noexcept;

  int  get_editing_width()  const noexcept{return m_editing_width;}
  int  get_editing_height() const noexcept{return m_editing_height;}

  int   get_pixel_size(      ) const noexcept{return m_pixel_size;}
  void  set_pixel_size(int  n)       noexcept;

  void  reset() noexcept;

  const gbstd::point&  get_current_point() const noexcept{return m_current_point;}

  void          set_drawing_color(gbstd::color  color)       noexcept{       m_drawing_color = color;}
  gbstd::color  get_drawing_color(                   ) const noexcept{return m_drawing_color        ;}

  drawing_recorder&  get_drawing_recorder() noexcept{return m_recorder;}

  void    set_grid() noexcept;
  void  unset_grid() noexcept;

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

  void  render(const gbstd::canvas&  cv) noexcept override;

  std::vector<uint8_t>  make_apng_stream(const std::vector<gbstd::point>&  pts, uint32_t  delay) const noexcept;

};






struct
context
{
  gbstd::image  m_source_image;

  int  m_cell_width;
  int  m_cell_height;

  int  m_table_width;
  int  m_table_height;

  gbstd::point  m_current_index;

  int  m_table_offset=0;


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

//  gbstd::menu*  m_menu;


  std::string  m_filepath;

  void  (*m_callback)();


  gbstd::rectangle  get_rect(gbstd::point  index) const noexcept;

  void  load(const std::vector<uint8_t>&  bin) noexcept;

  std::vector<gbstd::point>  m_animation_points;

  uint32_t  m_animation_delay=1000;

};


context*  create_graphics_editor(int  cell_w, int  cell_h, int  table_w, int  table_h) noexcept;




#endif




