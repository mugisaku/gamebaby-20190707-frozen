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
  case(directions::front):
      x = m_position.x;
      y = m_position.y;

      m_transformed_position = point3d(x,(uh-1-y)+(lh-1-z)-24,z);
      break;
  case(directions::right):
      x =    m_position.y;
      y = uh-m_position.x;

      m_transformed_position = point3d(x,(uh-1-y)+(lh-1-z),z);
      break;
  case(directions::back):
      x =  w-m_position.x;
      y = uh-m_position.y;

      m_transformed_position = point3d(x-24,(uh-1-y)+(lh-1-z),z);
      break;
  case(directions::left):
      x = w-m_position.y;
      y =   m_position.x;

      m_transformed_position = point3d(x-24,(uh-1-y)+(lh-1-z)-24,z);
      break;
    }
}


void
actor::
step(direction  dir) noexcept
{
    if(gbstd::g_time >= m_next_animation_time)
    {
      m_next_animation_time = gbstd::g_time+200;

      ++m_animation_counter;

      m_dirty_flag = true;
    }
}


void
actor::
render(const stack_map&  map, const gbstd::canvas&  cv) const noexcept
{
  auto  dst_pt = gbstd::point(m_transformed_position.x,m_transformed_position.y);

  gbstd::point  src_pt;

  auto  transformed_dir = m_dir+map.get_direction();

    switch(transformed_dir)
    {
  case(directions::front):
      src_pt = {0,0};
      break;
  case(directions::back):
      src_pt = {0,24};
      break;
  case(directions::left):
      src_pt = {24,0};
      break;
  case(directions::right):
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




