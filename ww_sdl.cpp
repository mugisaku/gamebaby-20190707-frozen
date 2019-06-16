#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libww/ww_context.hpp"
#include"sdl.hpp"
#include<cstring>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas
g_screen_canvas;


process
g_process;


ww::context
g_context;


void
main_loop() noexcept
{
  sdl::update_control();

    if(g_process.step())
    {
      sdl::update_screen(g_screen_canvas);
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

  sdl::init(g_context.get_screen_width(),g_context.get_screen_height(),1.0);

  g_screen_canvas = sdl::make_screen_canvas();

    if(argc == 2)
    {
        if(std::strcmp(argv[1],"-rec") == 0)
        {
          sdl::start_screen_recording();
        }
    }


  g_process.assign("system",g_context.make_entry())
    .set_interval(20)
    .set_canvas(g_screen_canvas);


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




