#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/file_op.hpp"
#include"sdl.hpp"
#include<list>
#include<vector>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


void
main_loop() noexcept
{
  constexpr int  delay = 80;

  static uint32_t  next;

  sdl::update_control();

    if(gbstd::g_time >= next)
    {
//      g_screen_canvas.fill(color());

//      sdl::update_screen(g_screen_canvas);

      next = gbstd::g_time+delay;
    }
}


}




int
main(int  argc, char**  argv)
{
/*
  constexpr int  screen_w = 320;
  constexpr int  screen_h = 320;

  sdl::init(screen_w,screen_h);

  g_screen_canvas = sdl::make_screen_canvas();

    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit();
*/
auto  s = make_string_from_file(argv[1]);
auto  p = reinterpret_cast<const uint8_t*>(s.data());
auto  end = reinterpret_cast<const uint8_t*>(s.data()+s.size());

while(p < end)
{
    if(ogg_page::test(p))
    {
      ogg_page  pg;

      p = pg.read(p);
    }

  else
    {
      ++p;
    }
}



  return 0;
}




