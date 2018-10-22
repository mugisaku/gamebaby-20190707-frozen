#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;




void
context::
resize_cell(gbstd::item_size  cell_size) noexcept
{
  resize_image(cell_size,m_menu->get_item_table_size());
}


void
context::
resize_table(gbstd::item_table_size  table_size) noexcept
{
  resize_image(m_menu->get_item_size(),table_size);
}


void
context::
resize_image(item_size  cell_size, item_table_size  table_size) noexcept
{
  auto&  w = cell_size.width ;
  auto&  h = cell_size.height;

  w = (w+7)/8*8;
  h = (h+7)/8*8;

  m_source_image.resize(w*table_size.x_length,h*table_size.y_length);

  m_menu->set_item_size({w,h});
  m_menu->set_item_table_size(table_size);
  m_menu->resize(5,2);


  revise();
}


void
context::
revise() noexcept
{
  m_core->rebase_canvas();

  m_core->rebase_underlay_stack();

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

      m_core->clear_underlay_stack();
      m_aniview->clear();

      resize_table(item_table_size{w/cell_w,h/cell_h});

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
context(item_size  cell_size, item_table_size  table_size) noexcept
{
  build_core();
  build_color_handler();
  build_bgcolor_changer();
  build_cell_table();

  m_aniview       = new aniview(*this);
  m_aniview_frame = new frame("animation",m_aniview);

  m_seamless_pattern_view       = new seamless_pattern_view(*this);
  m_seamless_pattern_view_frame = new frame("pattern",m_seamless_pattern_view);

  m_underlay_stacker       = new underlay_stacker(*this);
  m_underlay_stacker_frame = new frame("underlay",m_underlay_stacker);

  resize_image(cell_size,table_size);

  m_tool_widget_frame      = new frame(     "tool",m_core->create_tool_widget()     );
  m_operation_widget_frame = new frame("operation",m_core->create_operation_widget());

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




void
context::
build_color_handler() noexcept
{
  auto  color_list = {
    colors::black,
    colors::dark_gray,
    colors::gray,
    colors::light_gray,
    colors::white,
    colors::red,
    colors::green,
    colors::blue,
    colors::yellow,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
  };


  m_color_holder = new color_holder(color_list,[](color_holder&  holder, gbstd::color  color){
    auto&  ctx = *holder.get_userdata<context>();
    
    ctx.m_color_maker->set_color(color);
  });


  m_color_maker = new color_maker([](color_maker&  maker, gbstd::color  color){
    auto&  ctx = *maker.get_userdata<context>();

    ctx.m_color_holder->set_color(color);

    ctx.m_core->set_drawing_color(color);
  });


  m_color_maker->set_userdata(this);
  m_color_holder->set_userdata(this);

  m_color_maker_frame  = new frame("color",m_color_maker );
  m_color_holder_frame = new frame("palette",m_color_holder);
}


void
context::
build_bgcolor_changer() noexcept
{
  auto  ch1bg_btn = new button(new label(u"Change bg1 color",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_release())
      {
        auto  bgst = ctx.m_core->get_background_style();

        bgst.first_color = ctx.m_color_maker->get_color();

        ctx.m_core->set_background_style(bgst);

        ctx.m_core->request_redraw();
        ctx.m_menu->request_redraw();
      }
  });

  auto  ch2bg_btn = new button(new label(u"Change bg2 color",colors::black),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_release())
      {
        auto  bgst = ctx.m_core->get_background_style();

        bgst.second_color = ctx.m_color_maker->get_color();

        ctx.m_core->set_background_style(bgst);

        ctx.m_core->request_redraw();
        ctx.m_menu->request_redraw();
      }
  });


  ch1bg_btn->set_userdata(this);
  ch2bg_btn->set_userdata(this);

  m_bg_change_buttons = make_column({ch1bg_btn,ch2bg_btn});
}




}




