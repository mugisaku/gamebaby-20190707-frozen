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

          return;
        }
    }
}


void
fall_actor(gbstd::process&  proc, subiso::actor*  actor) noexcept
{
  int  n = 4;

    while(n--)
    {
      auto&  a = actor->m_current_position.z;
      auto   b = actor->m_next_position.z;

        if(a > b){--a;}
      else
        {
          actor->m_current_step_box = actor->m_next_step_box          ;
                                      actor->m_next_step_box = nullptr;

          proc.exit();

          return;
        }
    }
}


enum class
step_kind
{
  null,
  floor_to_floor,
  floor_to_up_stairs,
  floor_to_down_stairs,
  up_stairs_to_floor,
  up_stairs_to_up_stairs,
  down_stairs_to_floor,
  down_stairs_to_down_stairs,
};


struct
step_info
{
  step_kind  kind;

  box*  next_box;

  void  for_up_stairs(  subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept;
  void  for_down_stairs(subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept;
  void  for_stairs(     subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept;
  void  for_floor(      subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir                    ) noexcept;

  step_info(subiso::actor&  actor) noexcept;

};


void
step_info::
for_up_stairs(subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept
{
  auto  front_up_box = front_box.get_up_box();

    if(front_up_box && front_up_box->is_stairs() && (front_up_box->get_direction() == box_dir))
    {
      kind = step_kind::up_stairs_to_up_stairs;

      next_box = front_up_box;
    }

  else
    {
      kind = step_kind::up_stairs_to_floor;

      next_box = front_up_box;
    }
}


void
step_info::
for_down_stairs(subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept
{
    if(front_box.is_null())
    {
      auto  front_down_box = front_box.get_down_box();

        if(front_down_box && front_down_box->is_stairs() && (front_down_box->get_direction() == box_dir))
        {
          kind = step_kind::down_stairs_to_down_stairs;

          next_box = front_down_box;
        }

      else
        {
          kind = step_kind::down_stairs_to_floor;

          next_box = &front_box;
        }
    }
}


void
step_info::
for_stairs(subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept
{
    if(front_box.is_null() || (front_box.is_stairs() && (front_box.get_direction() == box_dir)))
    {
      kind = step_kind::floor_to_floor;

      next_box = &front_box;
    }
}


void
step_info::
for_floor(subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir) noexcept
{
    if(front_box.is_stairs())
    {
        if(front_box.get_direction() == ~actor_dir)
        {
          kind = step_kind::floor_to_up_stairs;

          next_box = &front_box;
        }
    }

  else
    if(front_box.is_null())
    {
      auto  front_down_box = front_box.get_down_box();

        if(front_down_box)
        {
            if(front_down_box->is_null())
            {
STEP_FOR_FALL:
                if(actor.get_flag_timer().test_flag())
                {
                  actor.get_flag_timer().clear();

                  kind = step_kind::floor_to_floor;

                  next_box = &front_box;
                }

              else
                if(!actor.get_flag_timer().is_active())
                {
                  actor.get_flag_timer().set_timer(600);
                }


              return;
            }

          else
            if(front_down_box->is_stairs())
            {
                if(front_down_box->get_direction() == actor_dir)
                {
                  kind = step_kind::floor_to_down_stairs;

                  next_box = front_down_box;

                  return;
                }

              else
                {
                  goto STEP_FOR_FALL;
                }
            }
        }


      kind = step_kind::floor_to_floor;

      next_box = &front_box;
    }
}


step_info::
step_info(subiso::actor&  actor) noexcept:
kind(step_kind::null),
next_box(nullptr)
{
  auto&  box = *actor.m_current_step_box;

  auto  actor_dir = actor.get_direction();

  box_view  bv(box,actor_dir);

  auto  front_box = bv.get_front_box();

    if(front_box)
    {
        if(box.is_stairs())
        {
          auto  box_dir = box.get_direction();

               if(box_dir ==  actor_dir){for_down_stairs(actor,bv,*front_box,actor_dir,box_dir);}
          else if(box_dir == ~actor_dir){for_up_stairs(  actor,bv,*front_box,actor_dir,box_dir);}
          else                          {for_stairs(     actor,bv,*front_box,actor_dir,box_dir);}
        }

      else
        {
          for_floor(actor,bv,*front_box,actor_dir);
        }
    }
}


void
start_fall(gbstd::process&  proc, subiso::actor*  actor, box&  down_box) noexcept
{
  actor->m_next_step_box = &down_box;

  actor->m_next_position.z = (g_plane_size*down_box.get_index().z);

    if(down_box.is_stairs())
    {
      actor->m_next_position.z += g_plane_size/2;
    }


  proc.get_foreground_job_list().add("fall player",fall_actor,40,actor);

  proc.step();
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

            if(down_box && (down_box->is_earth() || down_box->is_stairs()))
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


  auto  handler_dir = g_handler.get_direction();

    if(gbstd::g_input.test_shift())
    {
      static bool  shift_lock;

        if(!gbstd::g_input.test_left() &&
           !gbstd::g_input.test_right())
        {
          shift_lock = false;
        }


        if(!shift_lock)
        {
            if(gbstd::g_input.test_left())
            {
              g_handler.set_direction(handler_dir+1);

              shift_lock = true;
            }

          else
            if(gbstd::g_input.test_right())
            {
              g_handler.set_direction(handler_dir-1);

              shift_lock = true;
            }
        }

      return;
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
      actor->get_flag_timer().clear();

      return;
    }


  auto  transformed_dir = dir+handler_dir;

    if(actor->get_direction() != transformed_dir)
    {
      actor->get_flag_timer().clear();

      actor->set_direction(transformed_dir);
    }


  box_view  bv(box,transformed_dir);

  int  x = 0;
  int  y = 0;

    switch(transformed_dir)
    {
  case(directions::front): y = -1;break;
  case(directions::back ): y =  1;break;
  case(directions::left ): x = -1;break;
  case(directions::right): x =  1;break;
    }


  int   z = 0;
  int  zz = 0;

  step_info  inf(*actor);

    switch(inf.kind)
    {
  case(step_kind::null):
      return;
      break;
  case(step_kind::floor_to_floor):
      break;
  case(step_kind::floor_to_up_stairs):
      zz = 1;
      break;
  case(step_kind::floor_to_down_stairs):
      zz = -1;
      break;
  case(step_kind::up_stairs_to_floor):
      z = 1;
      break;
  case(step_kind::up_stairs_to_up_stairs):
       z = 1;
      zz = 1;
      break;
  case(step_kind::down_stairs_to_floor):
      z = -1;
      break;
  case(step_kind::down_stairs_to_down_stairs):
       z = -1;
      zz = -1;
      break;
    }


  actor->m_next_step_box = inf.next_box;

  actor->m_first_move_context.assign( x,y, z,g_plane_size/2);
  actor->m_second_move_context.assign(x,y,zz,g_plane_size/2);

  proc.get_foreground_job_list().add("move player",move_actor,20,actor);

  proc.step();
}



void
redraw_screen() noexcept
{
  g_screen_canvas.fill(color());

  g_handler.render(g_screen_canvas);

  auto&  map = g_handler.get_stack_map();

  g_actor.transform(map);

  g_actor.render(g_screen_canvas);
}


void
main_loop() noexcept
{
  sdl::update_control();

if(0)
{
    for(auto pt: gbstd::g_point_buffer)
    {
      auto&  map = g_handler.get_stack_map();

      auto&  s = map.get_stack(pt.x/g_plane_size,pt.y/g_plane_size);

      printf("%d\n",s.get_current()->get_image_z_base());
    }
}


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


  g_space.resize(8,8,6);

    for(int  y = 0;  y < g_space.get_y_length();  ++y){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,y,0).m_kind = subiso::box::kind::earth;
      g_space.get_box(x,y,1).m_kind = subiso::box::kind::earth;
    }}


  g_space.get_box(1,0,1).m_kind = subiso::box::kind::earth;
  g_space.get_box(1,1,1).m_kind = subiso::box::kind::earth;

  g_handler.assign(g_space);

  auto&  a_map = g_handler.get_stack_map(directions::front);
  auto&  b_map = g_handler.get_stack_map(directions::left );

  g_actor.set_space(&g_space);
  g_actor.set_current_step_box(&g_space.get_box(2,2,4));

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




