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


void
move_actor(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
  actor->step();

  int  n = 2;

    while(n--)
    {
        if(actor->m_first_move_context)
        {
          actor->m_first_move_context(actor->m_current_position);
        }

      else
        if(actor->m_second_move_context)
        {
          actor->m_second_move_context(actor->m_current_position);
        }

      else
        {
          actor->m_current_step_box = actor->m_next_step_box          ;
                                      actor->m_next_step_box = nullptr;

          proc.exit();
        }


      proc += 1;
    }
}


void
start_fall(gbstd::process&  proc, subiso::actor*  actor, box&  down_box) noexcept
{
  actor->m_next_step_box = &down_box;

  int   n = g_plane_size/2;
  int  nn = g_plane_size/2;

    if(down_box.is_stairs())
    {
      nn /= 2;
    }


  actor->m_first_move_context.assign( 0,0,-1, n);
  actor->m_second_move_context.assign(0,0,-1,nn);

  proc.get_foreground_job_list().add("fall player",move_actor,40,actor);

  proc.step();
}


void
start_move_from_stairs(gbstd::process&  proc, subiso::actor*  actor, const box_view&  bv, int  x, int  y) noexcept
{
  int   z = 0;
  int  zz = 0;

    if(actor->is_forward_direction())//to go down
    {
      auto  dst_box = bv.get_front_box();

        if(dst_box && !dst_box->is_earth())
        {
          actor->m_next_step_box = dst_box;

          z = 1;

            if(dst_box->is_stairs())
            {
              zz = 1;
            }


          actor->m_first_move_context.assign( x,y, z,g_plane_size/2);
          actor->m_second_move_context.assign(x,y,zz,g_plane_size/2);


          proc.get_foreground_job_list().add("move player",move_actor,40,actor);

          proc.step();
        }
    }

  else
    if(actor->is_reverse_direction())//to go up
    {
      auto  dst_box = bv.get_front_up_box();

        if(dst_box && !dst_box->is_earth())
        {
          actor->m_next_step_box = dst_box;

          z = 1;

            if(dst_box->is_stairs())
            {
              zz = 1;
            }


          actor->m_first_move_context.assign( x,y, z,g_plane_size/2);
          actor->m_second_move_context.assign(x,y,zz,g_plane_size/2);


          proc.get_foreground_job_list().add("move player",move_actor,40,actor);

          proc.step();
        }
    }

  else
    {
      auto  dst_box = bv.get_front_box();

        if(dst_box && dst_box->is_stairs() && (dst_box->get_direction() == bv.get_box()->get_direction()))
        {
          actor->m_next_step_box = dst_box;

          actor->m_first_move_context.assign( x,y,0,g_plane_size/2);
          actor->m_second_move_context.assign(x,y,0,g_plane_size/2);

          proc.get_foreground_job_list().add("move player",move_actor,40,actor);

          proc.step();
        }
    }
}


void
start_move_from_not_stairs(gbstd::process&  proc, subiso::actor*  actor, const box_view&  bv, int  x, int  y) noexcept
{
  auto  dst_box = bv.get_front_box();

    if(dst_box && !dst_box->is_earth())
    {
      int  z = 0;

        if(dst_box->is_stairs())
        {
            if(dst_box->get_direction() == ~actor->get_direction())
            {
              z = 1;
            }

          else
            {
              return;
            }
        }

      else
        if(dst_box->is_null())
        {
          auto  front_down_box = bv.get_front_down_box();

            if(front_down_box && front_down_box->is_stairs())
            {
              z = -1;

              dst_box = front_down_box;
            }
        }


      actor->m_next_step_box = dst_box;


      actor->m_first_move_context.assign( x,y,0,g_plane_size/2);
      actor->m_second_move_context.assign(x,y,z,g_plane_size/2);

      proc.get_foreground_job_list().add("move player",move_actor,40,actor);

      proc.step();
    }
}


void
change_front_box(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
  auto&  box = *actor->m_current_step_box;

    if(box.is_stairs())
    {
      return;
    }


  box_view  bv(box,actor->get_direction());

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
}


void
control_player(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
  actor->step();

  auto&  sp = *actor->get_space();

  space_view  sv(sp,directions::front);

    if(!actor->m_current_step_box)
    {
      auto&  pos = actor->m_current_position;

      actor->m_current_step_box = &sv.get_box(pos.x/g_plane_size,
                                              pos.y/g_plane_size,
                                              pos.z/g_plane_size);
    }


  auto&  box = *actor->m_current_step_box;

    {
      auto  dst_box = box.get_down_box();

        if(dst_box && !dst_box->is_earth())
        {
          start_fall(proc,actor,*dst_box);

          return;
        }
    }


  direction  dir;

       if(gbstd::g_input.test_up()   ){dir = directions::back ;}
  else if(gbstd::g_input.test_down() ){dir = directions::front;}
  else if(gbstd::g_input.test_left() ){dir = directions::left ;}
  else if(gbstd::g_input.test_right()){dir = directions::right;}
  else
    if(gbstd::g_input.test_p())
    {
      change_front_box(proc,actor);

      return;
    }

  else
    {
      return;
    }


  actor->set_direction(dir);

  box_view  bv(box,dir);

  int  x = 0;
  int  y = 0;

    switch(dir)
    {
  case(directions::front): y = -1;break;
  case(directions::back ): y =  1;break;
  case(directions::left ): x = -1;break;
  case(directions::right): x =  1;break;
    }


    if(box.is_stairs())
    {
      start_move_from_stairs(proc,actor,bv,x,y);
    }

  else
    {
      start_move_from_not_stairs(proc,actor,bv,x,y);
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




