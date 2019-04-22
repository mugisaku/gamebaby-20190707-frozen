#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/file_op.hpp"
#include"libgbstd/sound.hpp"
#include"libonch/onch.hpp"
#include"sdl.hpp"
#include<list>
#include<vector>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


constexpr int  g_num_keys = (100);

constexpr int  g_screen_w = g_num_keys*4;
constexpr int  g_screen_h = 240;

gbstd::canvas
g_screen_canvas;


void
draw_lines() noexcept
{
  g_screen_canvas.fill(gbstd::pixel());


  constexpr int  w = g_screen_w/g_num_keys;

    for(int  i = 0;  i < g_num_keys;  ++i)
    {
      g_screen_canvas.draw_rectangle(gbstd::colors::blue,i*w,0,w,g_screen_h);
    }


  sdl::update_screen(g_screen_canvas);
}


void
main_loop() noexcept
{
  static int  last_x_pos = -1;

  sdl::update_control();

    if(g_input.test_mouse_left())
    {
        if(gbstd::g_point_buffer.size())
        {
          last_x_pos = g_point_buffer.back().x/(g_screen_w/g_num_keys);

          sdl::set_keyon(last_x_pos);
        }
    }

  else
    {
      last_x_pos = -1;

      sdl::unset_keyon();
    }


    if(gbstd::g_dropped_file.size())
    {
    }
}
}




int
main(int  argc, char**  argv)
{
  sdl::init(g_screen_w,g_screen_h);

  sdl::init_sound(48000);

  sdl::start_sound_playing();

  sdl::make_wave_table();

  g_screen_canvas = sdl::make_screen_canvas();

  draw_lines();

#ifdef __EMSCRIPTEN__
  set_description("<pre>"
                  "</pre>");

  show_github_link();
  show_twitter_link();

  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }
#endif


  return 0;
}




