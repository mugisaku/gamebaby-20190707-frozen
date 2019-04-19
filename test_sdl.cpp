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
  sdl::init_sound(24000);

sdl::add_sound("test_bgm",R"(
drum = triangle{
  "p3-v70-f51"
  "r4"
  "p3-v70-f51"
  "r4"
  "p3-v70-f51"
  "p2-v70-f51"
  "r3"
  "p3-v70-f51"
  "r4"
}


f = text{
  "p-f3"
  "p-f5"
  "p-f4"
  "p-f3"
  "p-f4"
  "p-f5"
  "p-f6"

  "p-f2"
  "p-f4"
  "p-f3"
  "p-f2"
  "p-f3"
  "p-f4"
}

snd = sawtooth{
  "v3-b3"

  "p6-f6"
  "p5-f7"
  "p5-f6"
  "p5-f5"
  "p6-f6"
  "r3"

  "p3-f4"
  "p3-f5"
  "p3-f6"
  "p3-f5"
  "p3-f4"
  "p3-f3"
  "p4-f5"
  "r3"
  "p3-f4"
  "p-f3"
  "p-f5"
  "p-f4"
  "p-f3"
  "p-f4"
  "p-f5"
  "p-f6"

   f f

  "r3"

  "p5-f3"
  "r3"

  "p5-f5"
  "r3"

  "p6-f7"

}


drums = row{drum drum drum drum drum drum drum drum}


main = column{drums snd}
)");

sdl::add_sound("iikyoku",R"(main =
triangle{ "p4-v4-f5-b3"
 "p6-f6"
 "p4-f7"
 "p4-f6"
 "p4-f5"
 "p4-f4"
 "p6-f3"

 "r3"

 "p4-f4"
 "p6-f5"
 "p4-f6"
 "p4-f5"
 "p4-f4"
 "p6-f3"

 "r3"

 "p6-f3"
 "p4-f4"
 "p4-f3"
 "p4-f2"
 "p6-f1")");

sdl::add_sound("bashi","main = noise{\"p3-v4-f17-b3\"}");
sdl::change_bgm("test_bgm");

sdl::start_sound_playing();


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




