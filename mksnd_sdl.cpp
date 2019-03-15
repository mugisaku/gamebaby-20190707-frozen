#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/parser.hpp"
#include"sdl.hpp"
#include<list>
#include<vector>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


gbstd::canvas
g_screen_canvas;


namespace{


void
main_loop() noexcept
{
  constexpr int  delay = 80;

  static uint32_t  next;

  sdl::update_control();

    if(gbstd::g_time >= next)
    {
      g_screen_canvas.fill(color());

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

  sdl::init(200,100,1.0);
  sdl::init_sound();

  g_screen_canvas = sdl::make_screen_canvas();

  auto  s = gbstd::make_string_from_file("../music.txt");

  gbstd::tokenizer  tknz;

  auto  tokls = tknz(s.data());

  for(auto&  tok: tokls)
  {
    tok.print();
printf("\n");
  }
fflush(stdout);
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




