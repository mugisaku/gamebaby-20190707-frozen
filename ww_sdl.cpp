#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libww/ww_context.hpp"
#include"sdl.hpp"




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas
g_screen_canvas;


ww::context
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

      g_context.step(g_screen_canvas);

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

  sdl::init(g_context.get_screen_width(),g_context.get_screen_height(),1.0);
//  sdl::init_sound(24000);

//  sdl::add_sound("bashi","main = noise{\"p80:v20:f48:b3\"}");

//  sdl::start_sound_playing();


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




