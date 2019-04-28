#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/task.hpp"
#include"libgbstd/clock.hpp"
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


draw_task_list
g_list;


image
g_image(g_misc_png);


struct
data
{
  draw_task_list::control  ctrl;

  int  x=0;
  int  y=0;
};



std::list<data>
g_data;


void
cb(const canvas&  cv, data&  dat) noexcept
{
  cv.draw_canvas({g_image,0,0,24,24},dat.x,dat.y);
}


int  g_max;

clock_master
g_tmmaster;


void
add() noexcept
{
  g_data.emplace_back();

  auto&  dat = g_data.back();

  dat.ctrl = g_list.push({{},cb,dat});

//  dat.ctrl.set_blink_flag();
//  dat.ctrl.set_blinking_rate(10,20);

  dat.x = 100;
  dat.y = 100;

  g_max = std::max(g_max,(int)g_data.size());
}


void
main_loop() noexcept
{
  constexpr int  delay = 80;

  static uint32_t  next;

  sdl::update_control();

    if(g_input.test_up()){g_data.back().y -= 7;}
    if(g_input.test_down()){g_data.back().y += 7;}
    if(g_input.test_left()){g_data.back().x -= 7;}
    if(g_input.test_right()){g_data.back().x += 7;}
    if(g_input.test_p()){add();}
    if(g_input.test_n()){
if(g_data.size())
{
g_data.back().ctrl.set_remove_flag();
g_data.pop_back();
}
else
{
draw_task_list::print_dead();
printf(", max %d\n",g_max);

g_list.clear();
draw_task_list::clear_dead();

draw_task_list::print_dead();
printf(", max %d\n\n",g_max);
}

}
    if(gbstd::g_time >= next)
    {
      g_screen_canvas.fill(color());

      g_list.process(g_screen_canvas);

      sdl::update_screen(g_screen_canvas);

      next = gbstd::g_time+delay;

g_tmmaster.step();
g_tmmaster.print();
printf("\n");
    }
}


}




int
main(int  argc, char**  argv)
{
  constexpr int  screen_w = 320;
  constexpr int  screen_h = 320;

  sdl::init(screen_w,screen_h);

  g_screen_canvas = sdl::make_screen_canvas();
g_tmmaster+=gbstd::clock("4321",4321,true);
g_tmmaster+=gbstd::clock("2000",2000);
g_tmmaster+=gbstd::clock("1000",1000,true);
g_tmmaster+=gbstd::clock(" 500", 500,true);
g_tmmaster+=gbstd::clock(" 100", 100);
g_tmmaster+=gbstd::clock("   1",   1,true);
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit();


  return 0;
}




