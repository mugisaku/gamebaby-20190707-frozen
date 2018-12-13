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


image    g_bg_image;
canvas  g_bg_canvas;


process  g_base_process;

subiso::space          g_space;
subiso::space_handler  g_handler;
subiso::actor          g_actor;


void
redraw_bg() noexcept
{
  g_bg_canvas.fill(color());

  g_space.update();

  g_handler.render(g_bg_canvas);
}


void
move_actor(gbstd::execution&  exec, subiso::actor*  actor) noexcept
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

          exec.pop();

          return;
        }
    }
}


void
fall_actor(gbstd::execution&  exec, subiso::actor*  actor) noexcept
{
  int  n = 4;

    while(n--)
    {
      auto&  a = actor->m_current_position.z;
      auto   b = actor->m_next_position.z;

           if(a > b){--a;}
      else if(a < b){++a;}
      else
        {
          actor->m_current_step_box = actor->m_next_step_box          ;
                                      actor->m_next_step_box = nullptr;

          exec.pop();

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

  void  for_up_stairs(   subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept;
  void  for_down_stairs( subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir, direction  box_dir) noexcept;
  void  for_stairs(      subiso::actor&  actor, box_view&  bv, box&  front_box, direction  box_dir  ) noexcept;
  void  for_floor(       subiso::actor&  actor, box_view&  bv, box&  front_box, direction  actor_dir) noexcept;
  void  for_water_filled(subiso::actor&  actor, box_view&  bv, box&  front_box) noexcept;

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
for_stairs(subiso::actor&  actor, box_view&  bv, box&  front_box, direction  box_dir) noexcept
{
    if(front_box.is_null() || (front_box.is_stairs() && (front_box.get_direction() == box_dir)))
    {
      kind = step_kind::floor_to_floor;

      next_box = &front_box;
    }
}


void
step_info::
for_water_filled(subiso::actor&  actor, box_view&  bv, box&  front_box) noexcept
{
  auto  front_up_box = front_box.get_up_box();

    if(front_box.is_earth() && front_up_box && front_up_box->is_null())
    {
      kind = step_kind::up_stairs_to_floor;

      next_box = front_up_box;
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
          else                          {for_stairs(     actor,bv,*front_box,box_dir);}
        }

      else
        if(box.test_water_filled_flag())
        {
          for_water_filled(actor,bv,*front_box);
        }

      else
        {
          for_floor(actor,bv,*front_box,actor_dir);
        }
    }
}


void
change_front_box(subiso::actor*  actor) noexcept
{
  box_view  bv(*actor->m_current_step_box,actor->get_direction());

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

                if(down_box->is_contacted_water())
                {
                  down_box->flow_water();
                }
            }
        }

      else          
        if(front_box->is_earth())
        {
            if(actor->m_current_step_box->test_water_filled_flag())
            {
              front_box->be_null();

                if(front_box->is_contacted_water())
                {
                  front_box->flow_water();
                }
            }

          else
            {
              front_box->be_stairs(~actor->get_direction());

                if(front_box->is_contacted_water())
                {
                  front_box->flow_water();
                  front_box->update();
                }
            }
        }

      else          
        if(front_box->is_stairs())
        {
          front_box->be_null();
        }


      redraw_bg();
    }
}


void
add_new_box_to_front(subiso::actor*  actor) noexcept
{
  box_view  bv(*actor->m_current_step_box,actor->get_direction());

  auto  front_box = bv.get_front_box();

    if(front_box && front_box->is_null())
    {
      auto  previous_box = front_box                ;
      auto      down_box = front_box->get_down_box();

        while(down_box)
        {
            if(down_box->is_null())
            {
              previous_box = down_box                           ;
                             down_box = down_box->get_down_box();
            }

          else
            if(down_box->is_earth())
            {
              break;
            }

          else
            if(down_box->is_stairs())
            {
              previous_box = down_box                           ;

              break;
            }
        }


        if(!previous_box->test_water_filled_flag() && previous_box->is_surrounded_by_earth())
        {
          previous_box->set_water_source_flag();
          previous_box->set_water_filled_flag();
        }

      else
        {
          previous_box->be_earth();

          previous_box->unset_water_source_flag();
          previous_box->unset_water_filled_flag();
        }


      redraw_bg();
    }
}


void
control_player(gbstd::execution&  exec, subiso::actor*  actor) noexcept
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

    if(box.test_water_filled_flag())
    {
      auto  up_box = box.get_up_box();

        if(up_box && up_box->is_null() && up_box->test_water_filled_flag())
        {
          actor->m_next_position.z = up_box->get_ground_line();

          actor->m_next_step_box = up_box;

          exec.push(fall_actor);

          return;
        }
    }


    {
      auto  down_box = box.get_down_box();

        if(down_box && (down_box->is_null() || down_box->is_stairs()))
        {
          actor->m_next_position.z = down_box->get_ground_line();

          actor->m_next_step_box = down_box;

          exec.push(fall_actor);

          return;
        }
    }


    {
        if(actor->m_current_position.z != box.get_ground_line())
        {
          actor->m_next_position.z = box.get_ground_line();

          actor->m_next_step_box = &box;

          exec.push(fall_actor);

          return;
        }
    }


  auto  handler_dir = g_handler.get_direction();

  direction  dir;

    if(gbstd::g_input.test_shift())
    {
           if(gbstd::g_input.test_up()   ){dir = directions::back ;}
      else if(gbstd::g_input.test_down() ){dir = directions::front;}
      else if(gbstd::g_input.test_left() ){dir = directions::left ;}
      else if(gbstd::g_input.test_right()){dir = directions::right;}
      else{return;}


      actor->set_direction(dir+handler_dir);

      return;
    }


  static bool  lock;

       if(gbstd::g_input.test_up()   ){dir = directions::back ;}
  else if(gbstd::g_input.test_down() ){dir = directions::front;}
  else if(gbstd::g_input.test_left() ){dir = directions::left ;}
  else if(gbstd::g_input.test_right()){dir = directions::right;}
  else if(gbstd::g_input.test_start()){redraw_bg();return;}
  else if(gbstd::g_input.test_l())
    {
        if(!lock)
        {
          g_handler.set_direction(handler_dir-1);

          redraw_bg();

          lock = true;
        }


      return;
    }

  else
    if(gbstd::g_input.test_r())
    {
        if(!lock)
        {
          g_handler.set_direction(handler_dir+1);

          redraw_bg();

          lock = true;
        }


      return;
    }

  else
    if(gbstd::g_input.test_p())
    {
        if(!lock)
        {
          add_new_box_to_front(actor);

          lock = true;
        }


      return;
    }

  else
    if(gbstd::g_input.test_n())
    {
        if(!lock)
        {
          change_front_box(actor);

          lock = true;
        }


      return;
    }

  else
    {
      actor->get_flag_timer().clear();

      lock = false;

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

  exec.push(move_actor);
}




void
redraw_screen() noexcept
{
  auto&  map = g_handler.get_stack_map();

  g_actor.transform(map);


  g_screen_canvas.copy_canvas(g_bg_canvas,0,0);

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


  g_base_process.step();

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
                  "方向キーで移動、Shiftキーを押しながら方向キーで方向転換\n"
                  "前方に土BOXが無いとき、一時停止するが、長押しで進行\n"
                  "\n"
                  "Aキーを押すと、視点を左回転\n"
                  "Sキーを押すと、視点を右回転\n"
                  "\n"
                  "Enterキーを押すと、前方下に土BOXを生成する\n"
                  "四方が土BOXに囲まれているなら、水BOXになる\n"
                  "\n"
                  "Ctrlキーを押すと、前方に土BOXがあるとき、階段BOXに変化\n"
                  "階段BOXがあるとき、それを除去\n"
                  "なにもなければ、前方下のBOXを除去\n"
                  "\n"
                  "</pre>");

  show_github_link();
#endif


  g_space.resize(80,80,6);

    for(int  y = 0;  y < g_space.get_y_length();  ++y){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,y,0).m_kind = subiso::box::kind::earth;
    }}


  g_handler.assign(g_space);

  g_handler.set_view_size(200,200);

  g_actor.set_space(&g_space);
  g_actor.set_current_step_box(&g_space.get_box(2,2,4));

  g_base_process.assign("control player",20,control_player,&g_actor);


  sdl::init(g_handler.get_view_width(),g_handler.get_view_height());

  g_screen_canvas = sdl::make_screen_canvas();

  g_bg_image.resize(g_screen_canvas.get_width(),g_screen_canvas.get_height());
  g_bg_canvas = g_bg_image;

  redraw_bg();

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




