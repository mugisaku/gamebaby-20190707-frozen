#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/file_op.hpp"
#include"libgbstd/gadget.hpp"
#include"sdl.hpp"




using namespace gbstd;


namespace{


tile_source
g_source;


tile_map
g_map;


tile_map_view
g_map_view;


canvas
g_screen_canvas;


void
main_loop() noexcept
{
  constexpr int  delay = 80;

  static uint32_t  next;

  sdl::update_control();

constexpr int  v = 1;
if(g_input.test_up()   ){g_map_view.add_offset_y(-v);}
if(g_input.test_down() ){g_map_view.add_offset_y( v);}
if(g_input.test_left() ){g_map_view.add_offset_x(-v);}
if(g_input.test_right()){g_map_view.add_offset_x( v);}

    if(gbstd::g_time >= next)
    {
      g_screen_canvas.fill(color());

      g_map_view.render(g_screen_canvas);

      sdl::update_screen(g_screen_canvas);

      next = gbstd::g_time+delay;
    }
}


}




int
main(int  argc, char**  argv)
{
  constexpr int  screen_w = 320;
  constexpr int  screen_h = 320;

  constexpr int  tile_size = 16;

  sdl::init(screen_w,screen_h);

  g_screen_canvas = sdl::make_screen_canvas();

  g_source.make(tile_size,tile_size,4);

  auto  cv = g_source.get_canvas(0);

  cv.draw_rectangle(colors::white,0,0,tile_size,tile_size);
  cv.draw_rectangle(colors::black,1,1,tile_size-2,tile_size-2);
  cv.draw_string(colors::red,"A",0,0);

  g_map.resize(320/tile_size,320/tile_size);

  g_map_view.set_source(&g_source)
    .set_map(&g_map)
    .set_width(320)
    .set_height(320)
  ;

    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit();

  return 0;
}




