#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/value.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/window.hpp"
#include"libgbstd/windows/component.hpp"
#include"libsubiso/subiso.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;
using namespace subiso;


namespace{


canvas  g_screen_canvas;
process  g_root_process;

subiso::space          g_space;
subiso::space_handler  g_handler;
subiso::actor          g_actor;


constexpr int  g_number_of_steps = 4;
constexpr int  g_move_length = subiso::g_plane_size/g_number_of_steps;


void
move_left(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
    if(proc.get_pc() < g_move_length)
    {
      actor->m_position.x -= g_number_of_steps;
    }

  else
    {
     proc.exit();
    }


  proc.add_pc(1);
}


void
move_right(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
    if(proc.get_pc() < g_move_length)
    {
      actor->m_position.x += g_number_of_steps;
    }

  else
    {
      proc.exit();
    }


  proc.add_pc(1);
}


void
move_front(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
    if(proc.get_pc() < g_move_length)
    {
      actor->m_position.y += g_number_of_steps;
    }

  else
    {
      proc.exit();
    }


  proc.add_pc(1);
}


void
move_back(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
    if(proc.get_pc() < g_move_length)
    {
      actor->m_position.y -= g_number_of_steps;
    }

  else
    {
      proc.exit();
    }


  proc.add_pc(1);
}


void
move_up(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
    if(proc.get_pc() < g_move_length)
    {
      actor->m_position.z += g_number_of_steps;
    }

  else
    {
      proc.exit();
    }


  proc.add_pc(1);
}


void
move_down(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
    if(proc.get_pc() < g_move_length)
    {
      actor->m_position.z += -g_number_of_steps;
    }

  else
    {
      proc.exit();
    }


  proc.add_pc(1);
}


void
control_player(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
  auto&  sp = *actor->get_space();

  space_view  sv(sp,directions::front);

  auto&  pos = actor->m_position;

  auto&  box = sv.get_box(pos.x/g_plane_size,
                          pos.y/g_plane_size,
                          pos.z/g_plane_size);

  box_view  bv(box,actor->get_direction());

  auto  down_box = bv.get_down_box();

    if(down_box && down_box->is_null())
    {
      proc.get_foreground_job_list().add("fall player",move_down,40,actor);

      proc.step();

      return;
    }


  gbstd::point  pt;

  direction  dir;

  void  (*cb)(gbstd::process&  proc, subiso::actor*  actor) = nullptr;

    if(gbstd::g_input.test_shift())
    {
           if(gbstd::g_input.test_up()  ){cb = move_up;}
      else if(gbstd::g_input.test_down()){cb = move_down;}

      return;
    }

  else
    if(gbstd::g_input.test_up())
    {
      pt.y = 1;

      dir = directions::back;
    }

  else
    if(gbstd::g_input.test_down())
    {
      pt.y = -1;

      dir = directions::front;
    }

  else
    if(gbstd::g_input.test_left())
    {
      pt.x = -1;

      dir = directions::left;
    }

  else
    if(gbstd::g_input.test_right())
    {
      pt.x = 1;

      dir = directions::right;
    }

  else
    if(gbstd::g_input.test_p())
    {
      auto  front_box = bv.get_front_box();

        if(front_box)
        {
          bv.set_box(*front_box);

            if(front_box->is_null())
            {
              auto  down_box = bv.get_down_box();

                if(down_box && down_box->is_earth())
                {
                  down_box->be_null();
                }
            }

          else          
            if(front_box->is_earth())
            {
              front_box->be_stairs(~actor->get_direction());
            }
        }


      return;
    }

  else
    {
      return;
    }


  actor->set_direction(dir);

       if(pt.x < 0){cb = move_left ;}
  else if(pt.x > 0){cb = move_right;}
  else if(pt.y < 0){cb = move_back ;}
  else if(pt.y > 0){cb = move_front;}


    if(cb)
    {
      auto  front_box = bv.get_front_box();

        if(front_box)
        {
            if(front_box->is_null())
            {
              proc.get_foreground_job_list().add("move player",cb,40,actor);

              proc.step();
            }

          else
            if(front_box->is_stairs())
            {
//              proc.get_foreground_job() = gbstd::job(cb,40,actor);

//              proc.step();
            }
        }
    }
}



void
redraw_screen() noexcept
{
  g_screen_canvas.fill(color());

  g_handler.render(g_screen_canvas);

  auto&  map = g_handler.get_stack_map();

  g_actor.transform_position(map);

  g_actor.render(map,g_screen_canvas);
}


void
main_loop() noexcept
{
  sdl::update_control();

  g_root_process.step();

//    if(g_needed_to_redraw)
    {
      redraw_screen();

      sdl::update_screen(g_screen_canvas);
    }
}


}




int
main(int  argc, char**  argv)
{
#ifdef __EMSCRIPTEN__
  set_description("<pre>"
/*
                  "正方形の頂面と側面を等尺で、\n"
                  "二次元にレンダリングするプログラム\n"
                  "\n"
                  "適切な繋ぎ目を持つ画像に、自動で切り替わる\n"
                  "\n"
                  "\n"
*/
                  "カーソルキーで移動（XY値を変化）\n"
                  "Shiftキーを押しながら上下を押すと、Z値を変化\n"
//                  "マウスの左ボタンを押すと、ボックスを置く\n"
//                  "　　　　右ボタンを押すと、ボックスを消す\n"
                  "</pre>");

  show_github_link();
#endif


  g_space.resize(8,8,5);

    for(int  y = 0;  y < g_space.get_y_length();  ++y){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,y,0).m_kind = subiso::box::kind::earth;
    }}


  g_space.get_box(1,0,1).m_kind = subiso::box::kind::earth;
  g_space.get_box(1,1,1).m_kind = subiso::box::kind::earth;

  g_handler.assign(g_space);

  auto&  a_map = g_handler.get_stack_map(directions::front);
  auto&  b_map = g_handler.get_stack_map(directions::left );

  g_actor.set_space(&g_space);

  g_root_process.get_background_job_list().add("control player",control_player,80,&g_actor);


  sdl::init(a_map.get_image_width(),b_map.get_image_height());

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




