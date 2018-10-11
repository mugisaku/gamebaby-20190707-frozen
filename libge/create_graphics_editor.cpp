#include"libge/ge.hpp"




using namespace gbstd;


namespace{




void
update_table_offset_label(const context*  ctx) noexcept
{
  string_form  sf;

  int  num = ctx->m_table_offset/ctx->m_table_height;
  int  den = ctx->m_source_image.get_height()/(ctx->m_cell_height*ctx->m_table_height);

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
void
render_item(void*  userdata, const item_cursor&  cur,const canvas&  cv) noexcept
{
}
}


void
build_cell_table(context*  ctx) noexcept
{
  item_table_spec  spec = {ctx->m_cell_width,
                           ctx->m_cell_height,
                           ctx->m_table_width,
                           ctx->m_table_height,ctx,render_item};

  ctx->m_menu = new menu(spec,ctx->m_table_width,
                              ctx->m_table_height);

/*
  menu_item_parameter  mip = {0,0,
    [](menu&  menu, point  index, mouse_button  left, mouse_button  right)
    {
      auto  ctx = menu.get_userdata<context>();

        if(left)
        {
          ctx->m_current_index = point(0,ctx->m_table_offset)+index;

          ctx->m_core->set_cursor_offset(ctx->m_cell_width *ctx->m_current_index.x,
                                          ctx->m_cell_height*ctx->m_current_index.y);

          menu.request_redraw();

            if(ctx->m_callback)
            {
              ctx->m_callback();
            }
        }
    },

    [](menu&  menu, point  index, imactx_cursor  cur)
    {
      auto  ctx = menu.get_userdata<context>();

      auto  i = point(0,ctx->m_table_offset)+index;

      auto  src_rect = ctx->get_rect(i);

      imactxs::overlay(ctx->m_source_image,src_rect,cur);

        if(i == ctx->m_current_index)
        {
          cur.draw_rectangle(colors::white,0,0,ctx->m_cell_width,
                                               ctx->m_cell_height);
        }
    }
  };
*/

  ctx->m_menu->set_userdata(ctx);

  ctx->m_table_offset_label = new label(u" 1/ 1");

  auto  up_btn = new button(new iconshow({&icons::up}),[](button_event  evt){
  auto  ctx = evt->get_userdata<context>();

      if(evt.is_press())
      {
          if(ctx->m_table_offset >= ctx->m_table_height)
          {
            ctx->m_table_offset -= ctx->m_table_height;

            update_table_offset_label(ctx);

            ctx->m_menu->request_redraw();
          }
      }
  });

  auto  down_btn = new button(new iconshow({&icons::down}),[](button_event  evt){
  auto  ctx = evt->get_userdata<context>();

      if(evt.is_press())
      {
          if((ctx->m_table_offset+ctx->m_table_height) < (ctx->m_source_image.get_height()/ctx->m_cell_height))
          {
            ctx->m_table_offset += ctx->m_table_height;

            update_table_offset_label(ctx);

            ctx->m_menu->request_redraw();
          }
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


  //widget::set_userdata(ctx,{up_btn,down_btn,ext_btn});

  auto     btn_col = make_column({up_btn,down_btn,ext_btn});
  auto  celtbl_col = make_column({ctx->m_menu,ctx->m_table_offset_label});

  ctx->m_cell_table_frame = new frame("cell table",make_row({btn_col,celtbl_col}));
}



void
build_misc_buttons(context*  ctx) noexcept
{
  auto  ch1bg_btn = new button(new label(u"Chanctx bg1 color"),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_release())
      {
//        ctx->m_bg_style.set_first_color(ctx->m_color_maker->get_color());

        ctx->m_core->request_redraw();
//        ctx->m_menu->request_redraw();
      }
  });

  auto  ch2bg_btn = new button(new label(u"Chanctx bg2 color"),[](button_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_release())
      {
//        ctx->m_bg_style.set_second_color(ctx->m_color_maker->get_color());

        ctx->m_core->request_redraw();
//        ctx->m_menu->request_redraw();
      }
  });


  ch1bg_btn->set_userdata(ctx);
  ch2bg_btn->set_userdata(ctx);

  ctx->m_bg_change_buttons = make_column({ch1bg_btn,ch2bg_btn});
}



void
build_canvas(context*  ctx) noexcept
{
  ctx->m_cursor_label = new label(u"X: -- Y: -- PIX: ---");

  ctx->m_core = new core(ctx->m_source_image,ctx->m_cell_width,ctx->m_cell_height,[](core_event  evt){
    auto  ctx = evt->get_userdata<context>();

      if(evt.is_image_modified())
      {
//        ctx->m_menu->request_redraw();
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

  build_canvas(ctx);
  build_for_coloring(ctx);
  build_misc_buttons(ctx);
  build_cell_table(ctx);

  ctx->m_tool_widget_frame      = new frame(     "tool",ctx->m_core->create_tool_widget()     );
  ctx->m_operation_widget_frame = new frame("operation",ctx->m_core->create_operation_widget());

  ctx->m_core->set_cursor_offset(0,0);
  ctx->m_core->set_editing_size(cell_w,cell_h);

  ctx->m_source_image.resize(cell_w*table_w,cell_h*table_h);

//  ctx->m_menu->set_item_size(cell_w,cell_h);

//  ctx->m_core->set_style(ctx->m_bg_style);
//  ctx->m_menu->set_style(  ctx->m_bg_style);

  ctx->m_png_save_button = new button(new label(u"save as PNG"),[](button_event  evt){
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



  ctx->m_apng_save_button = new button(new label(u"save as APNG"),[](button_event  evt){
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




