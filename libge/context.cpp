#include"libge/ge_context.hpp"




namespace ge{


using namespace gbstd;




void
context::
resize_cell(gbstd::item_size  cell_size) noexcept
{
  resize_image(cell_size,m_menu->get_table_size());
}


void
context::
resize_table(gbstd::table_size  table_size) noexcept
{
  resize_image(m_menu->get_item_size(),table_size);
}


void
context::
resize_image(item_size  cell_size, table_size  table_size) noexcept
{
  auto&  w = cell_size.width ;
  auto&  h = cell_size.height;

//  w = (w+7)/8*8;
//  h = (h+7)/8*8;

  m_source_image.resize(w*table_size.x_length,h*table_size.y_length);

  m_menu->set_item_size({w,h});
  m_menu->set_table_size(table_size);
  m_menu->resize(5,2);


  revise();
}


void
context::
revise() noexcept
{
  auto  w = get_cell_width();
  auto  h = get_cell_height();

  gbstd::canvas  cv(m_source_image,w*m_current_index.x,h*m_current_index.y,w,h);

  m_paint.reset(*m_core);
  m_core->set_canvas(cv);

  m_core->get_display().rebase_underlay_stack(m_source_image,w,h);

  m_aniview->rebase();

  m_core->request_redraw();
  m_menu->request_redraw();
}


void
context::
load(const std::vector<uint8_t>&  bin) noexcept
{
  image  img;

  img.read_png_stream(bin.data());

    if(img.get_width())
    {
      int  cell_w = get_cell_width();
      int  cell_h = get_cell_height();

      int  w = std::max(img.get_width() ,cell_w*get_table_width() );
      int  h = std::max(img.get_height(),cell_h*get_table_height());

      w = (w+(cell_w-1))/cell_w*cell_w;
      h = (h+(cell_h-1))/cell_h*cell_h;

      m_current_index = point();

      m_core->get_display().clear_underlay_stack();
      m_aniview->clear();

      resize_table(table_size{w/cell_w,h/cell_h});

      m_source_image.fill(color());

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          m_source_image.get_pixel_pointer(x,y)->color = img.get_pixel_pointer(x,y)->color;
        }}


      revise();
    }
}


rectangle
context::
get_rect(point  index) const noexcept
{
   return rectangle(get_cell_width() *index.x,
                    get_cell_height()*index.y,
                    get_cell_width(),
                    get_cell_height());
}


context::
context(item_size  cell_size, table_size  table_size) noexcept
{
  build_core();
  build_cell_table();


  m_color_handler = new color_handler(m_paint);

  m_color_maker_frame  = new frame("color",m_color_handler->get_maker());
  m_color_holder_frame = new frame("palette",m_color_handler->get_holder());

  m_seamless_pattern_view       = new seamless_pattern_view(*this);
  m_seamless_pattern_view_frame = new frame("pattern",m_seamless_pattern_view);


  std::initializer_list<gbstd::widget*>  ls = {m_core,m_menu,m_seamless_pattern_view};

  m_core->set_affected_widget_list(ls);
  m_display.reset(m_color_handler->get_color(),ls);

  m_bg_changer = m_display.get_widget();

  m_aniview       = new aniview(*this);
  m_aniview_frame = new frame("animation",m_aniview);

  m_underlay_stacker       = new underlay_stacker(*this);
  m_underlay_stacker_frame = new frame("underlay",m_underlay_stacker);

  resize_image(cell_size,table_size);

  m_tool_widget_frame      = new frame(     "tool",m_paint.make_tool_widget()     );
  m_operation_widget_frame = new frame("operation",m_paint.make_operation_widget());

  m_png_save_button = new button(new label(u"save as PNG",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

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


  m_png_save_button->set_userdata(this);



  m_apng_save_button = new button(new label(u"save as APNG",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_release())
      {
        ctx.m_aniview->save_as_apng("noName.apng");
      }
  });


  m_apng_save_button->set_userdata(this);



  m_txt_save_button = new button(new label(u"save as C code",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

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


  m_txt_save_button->set_userdata(this);
}




}




