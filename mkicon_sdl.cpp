#include"libge/ge.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


core*
g_core;


canvas
g_canvas;


window*
g_window;


color_holder*
g_colhol;


color_maker*
g_colmak;


gbstd::image
g_image(g_icon_size,g_icon_size);


void
save(button_event  evt) noexcept
{
    if(evt.is_release())
    {
#ifdef EMSCRIPTEN
#else
        for(int  y = 0;  y < g_icon_size;  ++y)
        {
            for(int  x = 0;  x < g_icon_size;  ++x)
            {
              g_image.get_pixel_pointer(x,y)->color.print();

              printf(",");
            }


          printf("\n");
        }


      printf("\n");
#endif
    }
}


void
main_loop()
{
  sdl::update_control();

  g_window->do_total_reform_if_necessary();

    for(auto  pt: g_point_buffer)
    {
      g_window->process_user_input(pt);
    }


    if(g_window->redraw_if_necessary() || g_needed_to_redraw)
    {
      auto  cv = canvas(g_window->get_content_image());

      g_canvas.copy_canvas(cv,0,0);

      sdl::update_screen(g_canvas);
    }
}




}


int
main(int  argc, char**  argv)
{
  g_core = new core(g_image,g_icon_size,g_icon_size,nullptr);

  g_core->set_grid();
  g_core->set_pixel_size(12);


  auto  save_btn = new button(new label(u"SAVE",colors::black),save);

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


  g_colhol = new color_holder(color_list,[](color_holder&  holder, gbstd::color  color){
    g_colmak->set_color(color);

    g_core->set_drawing_color(color);
  });


  g_colmak = new color_maker([](color_maker&  maker, gbstd::color  color){
    g_colhol->set_color(color);

    g_core->set_drawing_color(color);
  });


  auto  color_maker_frame  = new frame("color",g_colmak);
  auto  color_holder_frame = new frame("palette",g_colhol);

  auto  mcol = make_column({color_maker_frame,g_core->create_operation_widget(),save_btn});


  auto cv_frame = new frame("canvas",g_core);

  g_window = new window();

  g_window->set_root_widget(make_row({cv_frame,color_holder_frame,mcol,g_core->create_tool_widget()}));

  sdl::init(g_window->get_width(),g_window->get_height());

  g_canvas = sdl::make_screen_canvas();

  sdl::update_screen(g_canvas);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  return 0;
}




