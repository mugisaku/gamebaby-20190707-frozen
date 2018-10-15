#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


void
context::
load(const std::vector<uint8_t>&  bin) noexcept
{
  image  img;

  img.read_png_stream(bin.data());

    if(img.get_width())
    {
      int  w = std::max(img.get_width() ,m_cell_width *m_table_width );
      int  h = std::max(img.get_height(),m_cell_height*m_table_height);

      w = (w+(m_cell_width -1))/m_cell_width *m_cell_width ;
      h = (h+(m_cell_height-1))/m_cell_height*m_cell_height;

      img.resize(w,h);

      m_current_index = point();

      m_core->clear_underlay_stack();
      m_aniview->clear();

      m_source_image = std::move(img);


      canvas  cv(m_source_image,0,0,m_cell_width,m_cell_height);

      m_core->set_canvas(cv);

      m_core->request_redraw();
      m_menu->request_redraw();

        if(m_callback)
        {
          m_callback();
        }
    }
}


rectangle
context::
get_rect(point  index) const noexcept
{
   return rectangle(m_cell_width *index.x,
                    m_cell_height*index.y,
                    m_cell_width,
                    m_cell_height);
}


context::
context(int  cell_w, int  cell_h, int  table_w, int  table_h) noexcept
{
  m_cell_width   = cell_w;
  m_cell_height  = cell_h;
  m_table_width  = table_w;
  m_table_height = table_h;

  m_source_image.resize(cell_w*table_w,cell_h*table_h);

  build_core();
  build_color_handler();
  build_bgcolor_changer();
  build_cell_table();

  m_aniview       = new aniview(*this);
  m_aniview_frame = new frame("animation",m_aniview);

  m_underlay_stacker       = new underlay_stacker(*this);
  m_underlay_stacker_frame = new frame("underlay",m_underlay_stacker);

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
//          if(ctx.m_animation.m_points.size())
          {
//            auto  bin = ctx.m_core->make_apng_stream(ctx.m_animation.m_points,ctx.m_animation.m_delay_ms);

//            constexpr const char*  filepath = "noName.apng";

#ifdef __EMSCRIPTEN__
//            download(bin.data(),bin.size(),filepath);
#else
//            write_to_file(bin.data(),bin.size(),filepath);
#endif
          }
      }
  });


  m_apng_save_button->set_userdata(this);
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




