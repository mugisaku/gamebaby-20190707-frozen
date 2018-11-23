#include"libsubiso/subiso.hpp"




namespace subiso{




namespace{


constexpr int  g_number_of_steps = 4;
constexpr int  g_move_length = g_plane_size/g_number_of_steps;


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

      dir = direction::back;
    }

  else
    if(gbstd::g_input.test_down())
    {
      pt.y = -1;

      dir = direction::front;
    }

  else
    if(gbstd::g_input.test_left())
    {
      pt.x = -1;

      dir = direction::left;
    }

  else
    if(gbstd::g_input.test_right())
    {
      pt.x = 1;

      dir = direction::right;
    }

  else
    {
      return;
    }


  auto&  hndl = *actor->get_handler();

  pt = transform(pt,hndl.get_direction());

  actor->set_direction(dir);

       if(pt.x < 0){cb = move_left ;}
  else if(pt.x > 0){cb = move_right;}
  else if(pt.y < 0){cb = move_back ;}
  else if(pt.y > 0){cb = move_front;}


    if(cb)
    {
      proc.get_foreground_job() = gbstd::job(cb,40,actor);

      proc.step();
    }
}
}


space_handler&
space_handler::
assign(space&  sp) noexcept
{
  m_front_map.assign(space_view(sp,direction::front));
  m_left_map.assign( space_view(sp,direction::left ));
  m_right_map.assign(space_view(sp,direction::right));
  m_back_map.assign( space_view(sp,direction::back ));

  m_current_map = &m_front_map;

  m_actor.m_handler = this;

  m_job_list.clear();

  m_job_list.add(gbstd::job(control_player,80,&m_actor));

  return *this;
}


const stack_map&
space_handler::
get_stack_map(direction  dir) const noexcept
{
  return (dir == direction::front)? m_front_map
        :(dir == direction::back )?  m_back_map
        :(dir == direction::left )?  m_left_map
        :                           m_right_map;
}


void
space_handler::
set_direction(direction  dir) noexcept
{
    switch(dir)
    {
  case(direction::front): m_current_map = &m_front_map;break;
  case(direction::back ): m_current_map = &m_back_map ;break;
  case(direction::left ): m_current_map = &m_left_map ;break;
  case(direction::right): m_current_map = &m_right_map;break;
    }
}


bool
space_handler::
change_direction_by_input() noexcept
{
    if(gbstd::g_input.test_left())
    {
      set_direction(get_left(get_direction()));

      return true;
    }

  else
    if(gbstd::g_input.test_right())
    {
      set_direction(get_right(get_direction()));

      return true;
    }


  return false;
}


void
space_handler::
step() noexcept
{
  static bool  lock;

    if(gbstd::g_input.test_shift())
    {
      constexpr uint32_t  interval = 400;

      static uint32_t  next_time;

        if(!lock)
        {
/*
            if(change_direction_by_input())
            {
              next_time = gbstd::g_time+interval;

              lock = true;

              set_dirty_flag();
            }
*/
        }

      else
        if(gbstd::g_time >= next_time)
        {
          lock = false;
        }
    }

  else
    {
      lock = false;
    }


  m_actor.step(get_direction());

  m_job_list.step();
}


void
space_handler::
render(const gbstd::canvas&  cv) noexcept
{
  int  w = m_current_map->get_width() ;
  int  h = m_current_map->get_height();

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  stack = m_current_map->get_stack(x,y);

      gbstd::canvas  cocv(cv,g_plane_size*x,g_plane_size*y,g_plane_size,g_plane_size);

      stack.render(cocv);
    }}


  m_actor.transform_position(*m_current_map);

  m_actor.render(*m_current_map,cv);

  m_dirty_flag = false;
}


void
space_handler::
render2(const gbstd::canvas&  cv) noexcept
{
  stack_map*  table[] = {&m_front_map,
    &m_right_map,&m_back_map,&m_left_map
  };

  int  x_base = 0;

    for(auto  map: table)
    {
      int  w = map->get_width() ;
      int  h = map->get_height();

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto&  stack = map->get_stack(x,y);

          gbstd::canvas  cocv(cv,x_base+(g_plane_size*x),g_plane_size*y,g_plane_size,g_plane_size);

          stack.render(cocv);
        }}


      gbstd::canvas  cocv(cv,x_base,0,g_plane_size*w,g_plane_size*h);

      m_actor.transform_position(*map);

      m_actor.render(*map,cocv);

      static gbstd::character_color  chcolor(gbstd::colors::black,gbstd::colors::white);

      gbstd::string_form  sf;

      gbstd::incremental  inc(0,16);

      cocv.draw_string(chcolor,sf("%s",get_name(map->get_direction()))     ,0,inc++);
      cocv.draw_string(chcolor,sf("X:%3d",m_actor.m_transformed_position.x),0,inc++);
      cocv.draw_string(chcolor,sf("Y:%3d",m_actor.m_transformed_position.y),0,inc++);
      cocv.draw_string(chcolor,sf("Z:%3d",m_actor.m_transformed_position.z),0,inc++);


      x_base += g_plane_size*w;
    }


  m_dirty_flag = false;
}




}




