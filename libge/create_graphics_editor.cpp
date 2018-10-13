#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


namespace{




void
update_table_offset_label(const context*  ctx) noexcept
{
  string_form  sf;

  int  num = ctx->m_menu->get_item_cursor().get_base().y;
  int  den = ctx->m_menu->get_item_table_size().y_length;

  ctx->m_table_offset_label->set_text(sf("%2d/%2d",num+1,den));
}


void
build_for_coloring(context*  ctx) noexcept
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


  ctx->m_color_holder = new color_holder(color_list,[](color_holder&  holder, gbstd::color  color){
    auto  ctx = holder.get_userdata<context>();
    
    ctx->m_color_maker->set_color(color);
  });


  ctx->m_color_maker = new color_maker([](color_maker&  maker, gbstd::color  color){
    auto  ctx = maker.get_userdata<context>();


    ctx->m_color_holder->set_color(color);

    ctx->m_core->set_drawing_color(color);
  });


  ctx->m_color_maker->set_userdata(ctx);
  ctx->m_color_holder->set_userdata(ctx);

  ctx->m_color_maker_frame  = new frame("color",ctx->m_color_maker );
  ctx->m_color_holder_frame = new frame("palette",ctx->m_color_holder);
}


namespace{
class
cell_menu: public menu
{
public:
  using menu::menu;

  void  render_background(const canvas&  cv) noexcept override
  {
    auto&  ctx = *get_userdata<context>();

    ctx.m_core->render_background(2,cv);
  }

  void  render_item(point  item_index, const canvas&  cv) noexcept override
  {
    auto&  ctx = *get_userdata<context>();

    int  w = ctx.m_cell_width ;
    int  h = ctx.m_cell_height;

    canvas  src_cv(ctx.m_source_image,w*item_index.x,h*item_index.y,
                                      w             ,h             );

    cv.draw_canvas(src_cv,0,0);

         if(ctx.m_current_index == item_index){cv.draw_rectangle(colors::white,0,0,w  ,h  );}
    else if(ctx.m_seeking_index == item_index){cv.draw_rectangle(colors::black,1,1,w-2,h-2);}
  }

};


void
process(menu_event  evt) noexcept
{
  auto&  ctx = *evt->get_userdata<context>();

    if(evt.is_press())
    {
      ctx.m_current_index = evt->get_item_cursor().get_result();

      auto&  pt = ctx.m_current_index;

      auto&  sz = evt->get_item_size();

      int  w = sz.width;
      int  h = sz.height;

      canvas  cv(ctx.m_source_image,w*pt.x,h*pt.y,w,h);

      ctx.m_core->set_canvas(cv);

        if(ctx.m_callback)
        {
          ctx.m_callback();
        }


      ctx.m_menu->request_redraw();
    }

  else
    if(evt.is_enter())
    {
      ctx.m_seeking_index = evt->get_item_cursor().get_result();

      ctx.m_menu->request_redraw();
    }
}
}


void
build_cell_table(context*  ctx) noexcept
{
  ctx->m_menu = new cell_menu(item_size{ctx->m_cell_width,ctx->m_cell_height},
                              item_table_size{ctx->m_table_width,ctx->m_table_height},
                              ctx->m_table_width,ctx->m_table_height,process);

  ctx->m_menu->set_userdata(ctx);


  ctx->m_table_offset_label = new label(u" 1/ 1",colors::black);

  auto  up_btn = new button(new iconshow({&icons::up}),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_press())
      {
        ctx->m_menu->move_up();

        update_table_offset_label(ctx);
      }
  });

  auto  down_btn = new button(new iconshow({&icons::down}),[](button_event  evt){
  auto  ctx = evt->get_userdata<context>();

      if(evt.is_press())
      {
        ctx->m_menu->move_down();

        update_table_offset_label(ctx);
      }
  });

  auto  ext_btn = new button(new iconshow({&icons::plus}),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_press())
      {
        ctx->m_source_image.resize(ctx->m_source_image.get_width(),
                                   ctx->m_source_image.get_height()+(ctx->m_cell_height*ctx->m_table_height));

        update_table_offset_label(ctx);
      }
  });


  gbstd::set_userdata({up_btn,down_btn,ext_btn},ctx);

  auto     btn_col = make_column({up_btn,down_btn,ext_btn});
  auto  celtbl_col = make_column({ctx->m_menu,ctx->m_table_offset_label});

  ctx->m_cell_table_frame = new frame("cell table",make_row({btn_col,celtbl_col}));
}



void
build_misc_buttons(context*  ctx) noexcept
{
  auto  ch1bg_btn = new button(new label(u"Change bg1 color",colors::black),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_release())
      {
        auto  bgst = ctx->m_core->get_background_style();

        bgst.first_color = ctx->m_color_maker->get_color();

        ctx->m_core->set_background_style(bgst);

        ctx->m_core->request_redraw();
        ctx->m_menu->request_redraw();
      }
  });

  auto  ch2bg_btn = new button(new label(u"Change bg2 color",colors::black),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_release())
      {
        auto  bgst = ctx->m_core->get_background_style();

        bgst.second_color = ctx->m_color_maker->get_color();

        ctx->m_core->set_background_style(bgst);

        ctx->m_core->request_redraw();
        ctx->m_menu->request_redraw();
      }
  });


  ch1bg_btn->set_userdata(ctx);
  ch2bg_btn->set_userdata(ctx);

  ctx->m_bg_change_buttons = make_column({ch1bg_btn,ch2bg_btn});
}




void
build_core(context*  ctx) noexcept
{
  ctx->m_cursor_label = new label(u"X: -- Y: -- PIX: ---",colors::black);

  gbstd::canvas  cv(ctx->m_source_image,0,0,ctx->m_cell_width,ctx->m_cell_height);

  ctx->m_core = new core(cv,[](core_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_image_modified())
      {
        ctx->m_menu->request_redraw();
        ctx->m_callback();
      }


    string_form  sf;

    auto&  pt = evt->get_current_point();

    auto  color = evt->get_pixel(pt.x,pt.y).color;

      if(color)
      {
        int  r = color.get_r7();
        int  g = color.get_g7();
        int  b = color.get_b7();

        sf("X: %2d Y: %2d PIX: %d%d%d",pt.x,pt.y,r,g,b);
      }

    else
      {
        sf("X: %2d Y: %2d PIX: ---",pt.x,pt.y);
      }


    ctx->m_cursor_label->modify_text(sf());
  });


  ctx->m_core->set_grid();
  ctx->m_core->set_pixel_size(8);
  ctx->m_core->set_userdata(ctx);


  ctx->m_core_frame = new frame("canvas",make_column({ctx->m_core,ctx->m_cursor_label}));

  ctx->m_core_frame->set_line_color(colors::white);
}


void
build_underlay(context*  ctx) noexcept
{
  auto  onpush = [](button_event  evt) noexcept{
      if(evt.is_release())
      {
        auto&  ctx = *evt->get_userdata<context>();

  //      g_point_stack.emplace_back(ctx.m_core->get_cursor_offset());

        string_form  sf;

        ctx.m_underlay.m_counter_label->set_text(sf("%2d",ctx.m_animation_points.size()));


        ctx.m_core->request_redraw();
//        animator::view.request_redraw();
      }
  };


  auto  onpop = [](button_event  evt) noexcept{
      if(evt.is_release())
      {
        auto&  ctx = *evt->get_userdata<context>();

          if(ctx.m_animation_points.size())
          {
            ctx.m_animation_points.pop_back();

            string_form  sf;

            ctx.m_underlay.m_counter_label->set_text(sf("%2d",ctx.m_animation_points.size()));


            ctx.m_core->request_redraw();
//            animator::view.request_redraw();
          }
      }
  };


  auto  onswitch = [](button_event  evt) noexcept{
      if(evt.is_release())
      {
        auto&  ctx = *evt->get_userdata<context>();

          if(ctx.m_core->test_whether_show_underlay())
          {
            ctx.m_core->hide_underlay();

            ctx.m_underlay.m_switching_label->modify_text(u"show");
          }

        else
          {
            ctx.m_core->show_underlay();

            ctx.m_underlay.m_switching_label->modify_text(u"hide");
          }


        ctx.m_core->request_redraw();
//        animator::view.request_redraw();
      }
  };


  ctx->m_underlay.m_switching_label = new label(u"hide",colors::black);
  ctx->m_underlay.m_counter_label   = new label(u" 0",colors::black);


  auto  psh_btn = new button(new label(u"push",colors::black),onpush);
  auto  pop_btn = new button(new label(u"pop",colors::black),onpop);
  auto  swi_btn = new button(ctx->m_underlay.m_switching_label,onswitch);

  set_userdata({psh_btn,pop_btn,swi_btn},ctx);


  auto  tbl = make_column({ctx->m_underlay.m_counter_label,psh_btn,pop_btn,swi_btn});

  ctx->m_underlay.m_frame = new frame("underlay",tbl);
}


}




void
context::
load(const std::vector<uint8_t>&  bin) noexcept
{
/*
  imactxs::imactx  img;

  img.read_png_stream(bin.data());

    if(img.get_width())
    {
      int  w = std::max(img.get_width() ,m_cell_width *m_table_width );
      int  h = std::max(img.get_height(),m_cell_height*m_table_height);

      w = (w+(m_cell_width -1))/m_cell_width *m_cell_width ;
      h = (h+(m_cell_height-1))/m_cell_height*m_cell_height;

      img.resize(w,h);

      m_table_offset = 0;

      m_source_image = std::move(img);

      m_core->request_redraw();
      m_menu->request_redraw();

        if(m_callback)
        {
          m_callback();
        }
    }
*/
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


context*
create_context(int  cell_w, int  cell_h, int  table_w, int  table_h) noexcept
{
  auto  ctx = new context;

  ctx->m_cell_width   = cell_w;
  ctx->m_cell_height  = cell_h;
  ctx->m_table_width  = table_w;
  ctx->m_table_height = table_h;

  ctx->m_source_image.resize(cell_w*table_w,cell_h*table_h);

  build_core(ctx);
  build_for_coloring(ctx);
  build_misc_buttons(ctx);
  build_cell_table(ctx);

  ctx->m_tool_widget_frame      = new frame(     "tool",ctx->m_core->create_tool_widget()     );
  ctx->m_operation_widget_frame = new frame("operation",ctx->m_core->create_operation_widget());

  ctx->m_png_save_button = new button(new label(u"save as PNG",colors::black),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_release())
      {
        auto  bin = ctx->m_source_image.make_png_stream();

        constexpr const char*  filepath = "noName.png";

#ifdef __EMSCRIPTEN__
        download(bin.data(),bin.size(),filepath);
#else
//        write_to_file(bin.data(),bin.size(),filepath);
#endif
      }
  });


  ctx->m_png_save_button->set_userdata(ctx);



  ctx->m_apng_save_button = new button(new label(u"save as APNG",colors::black),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_release())
      {
          if(ctx->m_animation_points.size())
          {
            auto  bin = ctx->m_core->make_apng_stream(ctx->m_animation_points,ctx->m_animation_delay);

            constexpr const char*  filepath = "noName.apng";

#ifdef __EMSCRIPTEN__
            download(bin.data(),bin.size(),filepath);
#else
//            write_to_file(bin.data(),bin.size(),filepath);
#endif
          }
      }
  });


  ctx->m_apng_save_button->set_userdata(ctx);


  return ctx;
}



}




