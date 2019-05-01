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


tick_task_list  g_tt_list;
draw_task_list  g_dt_list;


image
g_image(g_misc_png);


class
cursor
{
public:
  draw_task_control  dt_ctrl;
  tick_task_control  tt_ctrl;

  int  x=0;
  int  y=0;

  cursor(){}

};



std::list<cursor>
g_data;


void
dt_cb(const canvas&  cv, cursor&  dat) noexcept
{
  cv.draw_canvas({g_image,0,0,24,24},dat.x,dat.y);
}


void
tt_cb(cursor&  dat) noexcept
{
  ++dat.x;
  ++dat.y;
}


clock_master
g_tmmaster;


void
add() noexcept
{
  g_data.emplace_back();

  auto&  dat = g_data.back();
dat.x = 0;
dat.y = 0;
  dat.dt_ctrl = g_dt_list.push(dt_cb,dat);
  dat.tt_ctrl = g_tt_list.push(g_tmmaster["test"],100,tt_cb,dat);
}


void
print() noexcept
{
return;
      draw_task_list::print_dead();
      tick_task_list::print_dead();

      weak_reference_counter::print_dead();

      printf("\n");
}


void
main_loop() noexcept
{
  constexpr int  delay = 80;

  static uint32_t  next;

  sdl::update_control();

  g_tmmaster.step();

    if(g_data.size())
    {
      if(g_input.test_up()){g_data.back().y -= 7;}
      if(g_input.test_down()){g_data.back().y += 7;}
      if(g_input.test_left()){g_data.back().x -= 7;}
      if(g_input.test_right()){g_data.back().x += 7;}
    }


    if(g_input.test_p() && g_modified_input.test_p())
    {
      add();
//      print();
    }


    if(g_input.test_n() && g_modified_input.test_n())
    {
        if(g_data.size())
        {
          g_data.back().dt_ctrl.set_remove_flag();
          g_data.back().tt_ctrl.set_remove_flag();
          g_data.pop_back();
        }

      print();
    }


    if(gbstd::g_time >= next)
    {
      g_screen_canvas.fill(color());

      g_tt_list.process();
      g_dt_list.process(g_screen_canvas);

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

  sdl::init(screen_w,screen_h);

  g_screen_canvas = sdl::make_screen_canvas();

  auto  ctrl = g_tmmaster.add("test",1000);

  ctrl.start();

    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit();


  return 0;
}




