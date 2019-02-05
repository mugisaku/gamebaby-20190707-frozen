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


constexpr uint32_t  tmstd=80;


ww::battle_context
g_context(tmstd);


void
main_loop() noexcept
{
  static uint32_t  next;

  sdl::update_control();

    if(gbstd::g_time >= next)
    {
      g_screen_canvas.fill(color());

      g_context.step();

      g_context.render(g_screen_canvas);

      sdl::update_screen(g_screen_canvas);

      next = gbstd::g_time+tmstd;
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

  ww::force_initializer  l;
  ww::force_initializer  r;

  static ww::company  l_companies[] = {
    {"ひだりぐんA",3000,ww::front_pos},
    {"ひだりぐんB",3000,ww::front_pos},
    {"ひだりぐんC",3000,ww::front_pos},
  };

  static ww::company  r_companies[] = {
    {"みぎぐんA",3000,ww::front_pos},
    {"みぎぐんB",3000,ww::back_pos},
    {"みぎぐんC",3000,ww::front_pos},
  };


  l.m_color = colors::red;
  r.m_color = colors::blue;

  l.add(l_companies,3);
  r.add(r_companies,3);

  g_context.set_field_size(screen_w,screen_h);

  g_context.reset(l,r);

  g_context.show_text();

  g_screen_canvas = sdl::make_screen_canvas();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




