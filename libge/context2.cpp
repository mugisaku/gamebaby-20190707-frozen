#include"libge/ge_context.hpp"




namespace ge{


using namespace gbstd;


constexpr int  g_cell_size = 12;


const char16_t*
name_table[] =
{
  u"base      A",
  u"base      B",
  u"edge    TOP",
  u"edge BOTTOM",
  u"edge   LEFT",
  u"edge  RIGHT",
};


constexpr int  g_n_items = 6;


namespace{
class
parts_menu: public menu
{
public:
  using menu::menu;

  void  render_background(const canvas&  cv) noexcept override
  {
    auto&  ctx = *get_userdata<context2>();

    cv.fill(color());

    ctx.m_core->get_display().render_background({cv,0,0,g_cell_size,cv.get_height()},2);
  }

  void  render_item(point  item_index, const canvas&  cv) noexcept override
  {
    auto&  ctx = *get_userdata<context2>();

    canvas  src_cv(ctx.m_source_image,g_cell_size*item_index.y,g_cell_size*item_index.x,
                                      g_cell_size             ,g_cell_size             );

    cv.draw_canvas(src_cv,0,0);

    cv.draw_string(character_color(colors::white),name_table[item_index.y],g_cell_size,0);

      if(ctx.m_current_index == item_index)
      {
        cv.draw_rectangle(colors::white,0,0,cv.get_width(),cv.get_height());
      }
  }

};


void
process(menu_event  evt) noexcept
{
  auto&  ctx = *evt->get_userdata<context2>();

    if(evt.is_press())
    {
      ctx.m_current_index = evt->get_item_cursor().get_result();

      auto&  pt = ctx.m_current_index;

      gbstd::canvas  cv(ctx.m_source_image,g_cell_size*pt.y,g_cell_size*pt.x,g_cell_size,g_cell_size);

      ctx.m_core->set_canvas(cv);

      ctx.m_core->request_redraw_all();
    }

  else
    if(evt.is_enter())
    {
    }
}
}




class
preview: public gbstd::widget
{
  gbstd::canvas  m_a_base;
  gbstd::canvas  m_b_base;
  gbstd::canvas  m_top_edge;
  gbstd::canvas  m_bottom_edge;
  gbstd::canvas  m_left_edge;
  gbstd::canvas  m_right_edge;

  static constexpr int     top = 1;
  static constexpr int  bottom = 2;
  static constexpr int    left = 4;
  static constexpr int   right = 8;

public:
  preview(const image&  img) noexcept;

  void  process_before_reform() noexcept override;

  void  render_product(point  pt, int  flags, const gbstd::canvas&  cv) noexcept;

  void  render(const gbstd::canvas&  cv) noexcept override;

};


preview::
preview(const image&  img) noexcept:
m_a_base(     img,g_cell_size*0,0,g_cell_size,g_cell_size),
m_b_base(     img,g_cell_size*1,0,g_cell_size,g_cell_size),
m_top_edge(   img,g_cell_size*2,0,g_cell_size,g_cell_size),
m_bottom_edge(img,g_cell_size*3,0,g_cell_size,g_cell_size),
m_left_edge(  img,g_cell_size*4,0,g_cell_size,g_cell_size),
m_right_edge( img,g_cell_size*5,0,g_cell_size,g_cell_size)
{
}




void
preview::
process_before_reform() noexcept
{
  auto&  ctx = *get_userdata<context>();

  set_content_width( g_cell_size*2*5);
  set_content_height(g_cell_size*2*5);
}


void
preview::
render_product(point  pt, int  flags, const gbstd::canvas&  cv) noexcept
{
  int  x = g_cell_size*2*pt.x;
  int  y = g_cell_size*2*pt.y;

  cv.draw_canvas(m_a_base,x            ,y            );
  cv.draw_canvas(m_b_base,x+g_cell_size,y            );
  cv.draw_canvas(m_b_base,x            ,y+g_cell_size);
  cv.draw_canvas(m_a_base,x+g_cell_size,y+g_cell_size);

    if(flags&left)
    {
      cv.draw_canvas(m_left_edge,x,y            );
      cv.draw_canvas(m_left_edge,x,y+g_cell_size);
    }


    if(flags&right)
    {
      cv.draw_canvas(m_right_edge,x+g_cell_size,y            );
      cv.draw_canvas(m_right_edge,x+g_cell_size,y+g_cell_size);
    }


    if(flags&top)
    {
      cv.draw_canvas(m_top_edge,x            ,y);
      cv.draw_canvas(m_top_edge,x+g_cell_size,y);
    }


    if(flags&bottom)
    {
      cv.draw_canvas(m_bottom_edge,x            ,y+g_cell_size);
      cv.draw_canvas(m_bottom_edge,x+g_cell_size,y+g_cell_size);
    }
}


void
preview::
render(const canvas&  cv) noexcept
{
  auto&  ctx = *get_userdata<context2>();

  cv.fill(color());

  ctx.m_core->get_display().render_background(cv,2);

  render_product({0,0},top|left,cv);
  render_product({1,0},top,cv);
  render_product({2,0},top|right,cv);
  render_product({0,1},left,cv);
  render_product({1,1},0,cv);
  render_product({2,1},right,cv);
  render_product({0,2},bottom|left,cv);
  render_product({1,2},bottom,cv);
  render_product({2,2},bottom|right,cv);

  render_product({3,0},bottom|top,cv);
  render_product({4,0},left|right,cv);

  render_product({3,1},bottom|top|left,cv);
  render_product({4,1},bottom|top|right,cv);

  render_product({3,2},bottom|left|right,cv);
  render_product({4,2},top|left|right,cv);

  render_product({0,3},top|bottom|left|right,cv);
}




context2::
context2() noexcept
{
  constexpr int   cell_size = 12;
  constexpr int  pixel_size = 12;

  m_color_handler = new color_handler(m_paint);

  m_color_maker_frame  = new frame("color",m_color_handler->get_maker());
  m_color_holder_frame = new frame("palette",m_color_handler->get_holder());


  m_tool_widget_frame      = new frame(     "tool",m_paint.make_tool_widget()     );
  m_operation_widget_frame = new frame("operation",m_paint.make_operation_widget());

  m_source_image.resize(cell_size*6,cell_size);

  m_display.set_pixel_size(pixel_size);

  m_core = new core(m_paint,m_display,nullptr);

  m_paint.reset(*m_core);

  m_core_frame = new frame("canvas",m_core);


  m_menu = new parts_menu(1,6,process);

  m_menu->set_item_size({g_cell_size+(g_font_width*12),g_font_height});
  m_menu->set_table_size({1,6});

  m_menu->set_userdata(this);

  m_menu_frame = new frame("parts menu",m_menu);

  auto  prev = new preview(m_source_image);

  prev->set_userdata(this);

  m_preview_frame = new frame("preview",prev);

  std::initializer_list<widget*>  ls = {m_core,m_menu,prev};

  m_core->set_canvas({m_source_image,0,0,cell_size,cell_size});
  m_core->set_affected_widget_list(ls);

  m_display.reset(m_color_handler->get_color(),ls);

  m_bg_changer = m_display.get_widget();


  m_parts_save_button = new button(new label(u"save parts as PNG",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context2>();

      if(evt.is_release())
      {
        auto  bin = ctx.m_source_image.make_png_stream();

        constexpr const char*  filepath = "noName.png";

#ifdef __EMSCRIPTEN__
        download(bin.data(),bin.size(),filepath);
#else
        write_to_file(bin.data(),bin.size(),filepath);
#endif
      }
  });


  m_result_save_button = new button(new label(u"save result as PNG",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context2>();

      if(evt.is_release())
      {
        auto  bin = ctx.m_source_image.make_png_stream();

        constexpr const char*  filepath = "noName.png";

#ifdef __EMSCRIPTEN__
        download(bin.data(),bin.size(),filepath);
#else
        write_to_file(bin.data(),bin.size(),filepath);
#endif
      }
  });


  m_txt_save_button = new button(new label(u"save result as TXT",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context2>();

      if(evt.is_release())
      {
        auto  txt = ctx.m_source_image.make_txt_stream();

        constexpr const char*  filepath = "noName.txt";

#ifdef __EMSCRIPTEN__
        download(txt.data(),txt.size(),filepath);
#else
        write_to_file(txt.data(),txt.size(),filepath);
#endif
      }
  });


  set_userdata({m_parts_save_button,m_result_save_button},this);
}




}




