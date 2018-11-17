#include"libsubiso/subiso.hpp"




namespace subiso{




space_handler&
space_handler::
assign(space&  sp) noexcept
{
  m_front_map.assign(space_view(sp,direction::front));
  m_left_map.assign( space_view(sp,direction::left ));
  m_right_map.assign(space_view(sp,direction::right));
  m_back_map.assign( space_view(sp,direction::back ));

  m_dir = direction::front;

  m_current_map = &m_front_map;

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
  m_dir = dir;

    switch(dir)
    {
  case(direction::front): m_current_map = &m_front_map;break;
  case(direction::back ): m_current_map = &m_back_map ;break;
  case(direction::left ): m_current_map = &m_left_map ;break;
  case(direction::right): m_current_map = &m_right_map;break;
    }
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
}




}




