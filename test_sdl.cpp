#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libww/ww.hpp"
#include"sdl.hpp"
#include<list>
#include<vector>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas
g_screen_canvas;


ww::executive_context
g_context;


void
main_loop() noexcept
{
  constexpr int  delay = 80;

  static uint32_t  next;

  sdl::update_control();

    if(gbstd::g_time >= next)
    {
      g_screen_canvas.fill(color());

      gbstd::process_task_lists();

      gbstd::render_painter_lists(g_screen_canvas);

      sdl::update_screen(g_screen_canvas);

      next = gbstd::g_time+delay;
    }
}


}




int
main(int  argc, char**  argv)
{
#ifdef __EMSCRIPTEN__
  set_description("<pre>"
                  "</pre>");

  show_github_link();
#endif

  constexpr int  screen_w = ww::g_center_space_width+(ww::g_row_width*2);
  constexpr int  screen_h = (ww::g_column_height);

  sdl::init(screen_w,screen_h,1.0);
  sdl::init_sound();

  g_context.set_screen_size(screen_w,screen_h);

  g_context.startup();

  g_screen_canvas = sdl::make_screen_canvas();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit_sound();
  sdl::quit();
#endif


  return 0;
}




