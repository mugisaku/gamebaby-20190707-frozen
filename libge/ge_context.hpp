#ifndef ge_context_hpp_was_included
#define ge_context_hpp_was_included


#include"libge/ge.hpp"


namespace ge{


struct
context
{
  gbstd::image  m_source_image;

  gbstd::point  m_current_index;
  gbstd::point  m_seeking_index;


  core_display  m_display;
  core_paint      m_paint;

  core*  m_core;

  gbstd::frame*   m_core_frame;

  gbstd::frame*   m_cell_table_frame;

  color_handler*  m_color_handler;

  gbstd::frame*  m_color_maker_frame;
  gbstd::frame*  m_color_holder_frame;

  gbstd::frame*  m_tool_widget_frame;
  gbstd::frame*  m_operation_widget_frame;

  gbstd::widget*  m_bg_changer;

  gbstd::label*  m_cursor_label;
  gbstd::label*  m_table_offset_label;

  gbstd::widget*  m_seamless_pattern_view;
  gbstd::frame*   m_seamless_pattern_view_frame;

  gbstd::button*   m_png_save_button;
  gbstd::button*   m_txt_save_button;
  gbstd::button*  m_apng_save_button;

  gbstd::menu*  m_menu;


  aniview*  m_aniview=nullptr;
  gbstd::frame*  m_aniview_frame;

  underlay_stacker*  m_underlay_stacker=nullptr;
  gbstd::frame*      m_underlay_stacker_frame;

  std::string  m_filepath;


  gbstd::rectangle  get_rect(gbstd::point  index) const noexcept;

  void  load(const std::vector<uint8_t>&  bin) noexcept;

  context(gbstd::item_size  cell_size, gbstd::table_size  table_size) noexcept;

  void  build_core() noexcept;
  void  build_cell_table() noexcept;
  void  build_animation() noexcept;
  void  build_underlay() noexcept;

  void  update_table_offset_label() noexcept;

  void  resize_cell(gbstd::item_size  cell_size) noexcept;
  void  resize_table(gbstd::table_size  table_size) noexcept;
  void  resize_image(gbstd::item_size  cell_size, gbstd::table_size  table_size) noexcept;

  void  revise() noexcept;

  int  get_cell_width()  const noexcept{return m_menu->get_item_size().width ;}
  int  get_cell_height() const noexcept{return m_menu->get_item_size().height;}

  int  get_table_width()  const noexcept{return m_menu->get_table_size().x_length;}
  int  get_table_height() const noexcept{return m_menu->get_table_size().y_length;}

};




struct
context2
{
  gbstd::image  m_source_image;

  core_display  m_display;
  core_paint      m_paint;

  core*  m_core;

  gbstd::frame*   m_core_frame;

  gbstd::menu*  m_menu;

  gbstd::frame*   m_menu_frame;
  gbstd::frame*   m_preview_frame;

  gbstd::point  m_current_index;

  color_handler*  m_color_handler;

  gbstd::frame*  m_color_maker_frame;
  gbstd::frame*  m_color_holder_frame;

  gbstd::frame*  m_tool_widget_frame;
  gbstd::frame*  m_operation_widget_frame;

  gbstd::widget*  m_bg_changer;

  gbstd::label*  m_cursor_label;

  context2() noexcept;

};


}


#endif




