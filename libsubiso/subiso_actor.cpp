#include"libsubiso/subiso.hpp"




namespace subiso{


namespace{
const uint8_t
g_binary[] =
{
#include"actor.txt"
};


gbstd::image
g_image(g_binary);
}




actor::
actor() noexcept: m_position(0,0,24)
{
}




void
actor::
transform_position(const stack_map&  map) noexcept
{
  int   w = map.get_image_width() ;
  int  uh = map.get_upper_image_height();
  int  lh = map.get_lower_image_height();
  int   h = uh+lh;

  int  x;
  int  y;
  int  z = m_position.z;

    switch(map.get_direction())
    {
  case(direction::front):
      x = m_position.x;
      y = m_position.y;

      m_transformed_position = point3d(x,(uh-1-y)+(lh-1-z)-24,z);
      break;
  case(direction::right):
      x =    m_position.y;
      y = uh-m_position.x;

      m_transformed_position = point3d(x,(uh-1-y)+(lh-1-z),z);
      break;
  case(direction::back):
      x =  w-m_position.x;
      y = uh-m_position.y;

      m_transformed_position = point3d(x-24,(uh-1-y)+(lh-1-z),z);
      break;
  case(direction::left):
      x = w-m_position.y;
      y =   m_position.x;

      m_transformed_position = point3d(x-24,(uh-1-y)+(lh-1-z)-24,z);
      break;
    }
}


void
actor::
process_input(direction  dir) noexcept
{
  constexpr int  stroke = 1;

  gbstd::point  pt;

    if(gbstd::g_input.test_shift())
    {
        if(gbstd::g_input.test_up())
        {
          m_position.z += stroke;
        }

      else
        if(gbstd::g_input.test_down())
        {
          m_position.z += -stroke;
        }


      return;
    }

  else
    if(gbstd::g_input.test_up())
    {
      pt.y = stroke;

      m_dir = direction::back;
    }

  else
    if(gbstd::g_input.test_down())
    {
      pt.y = -stroke;

      m_dir = direction::front;
    }

  else
    if(gbstd::g_input.test_left())
    {
      pt.x = -stroke;

      m_dir = direction::left;
    }

  else
    if(gbstd::g_input.test_right())
    {
      pt.x = stroke;

      m_dir = direction::right;
    }

  else
    {
      return;
    }


  pt = transform(pt,dir);

//  m_dir = transform(m_dir,dir);

  m_position.x += pt.x;
  m_position.y += pt.y;
}


void
actor::
step(direction  dir) noexcept
{
//    if(gbstd::g_time >= m_next_animation_time)
    {
      m_next_animation_time = gbstd::g_time+120;

      ++m_animation_counter;

      process_input(dir);

      m_handler->set_dirty_flag();
    }
}


void
actor::
render(const stack_map&  map, const gbstd::canvas&  cv) const noexcept
{
  auto  dst_pt = gbstd::point(m_transformed_position.x,m_transformed_position.y);

  gbstd::point  src_pt;

  auto  transformed_dir = transform(m_dir,map.get_direction());

    switch(transformed_dir)
    {
  case(direction::front):
      src_pt = {0,0};
      break;
  case(direction::back):
      src_pt = {0,24};
      break;
  case(direction::left):
      src_pt = {24,0};
      break;
  case(direction::right):
      src_pt = {24,24};
      break;
    }


  int  n = m_animation_counter%4;

    switch(n)
    {
  case(1): src_pt.x += 48*1;break;
  case(3): src_pt.x += 48*2;break;
    }


  gbstd::canvas  src_cv(g_image,src_pt.x,src_pt.y,24,24);

  cv.blend_canvas(src_cv,dst_pt.x,dst_pt.y,m_transformed_position.z+24,-1);
}




}




