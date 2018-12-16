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
actor() noexcept
{
}




void
actor::
set_current_step_box(subiso::box*  box) noexcept
{
  m_current_step_box = box;

    if(box)
    {
      auto&  i = box->get_index();

      m_current_position.x = (g_plane_size*i.x)+12;
      m_current_position.y = (g_plane_size*i.y)+12;
      m_current_position.z = (g_plane_size*i.z);
    }
}


void
actor::
step() noexcept
{
  m_flag_timer.update();

    if(gbstd::g_time >= m_next_animation_time)
    {
      m_next_animation_time = gbstd::g_time+200;

      ++m_animation_counter;

      m_dirty_flag = true;
    }
}


void
actor::
transform(const stack_map&  map) noexcept
{
  m_transformed_position = map.transform(m_current_position);

  m_transformed_dir = m_dir-map.get_direction();
}


void
actor::
render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept
{
  auto  dst_pt = m_transformed_position+offset+gbstd::point(cv.get_width()/2,cv.get_height()/2);

  gbstd::point  src_pt;

    switch(m_transformed_dir)
    {
  case(directions::front): src_pt = { 0, 0};break;
  case(directions::back ): src_pt = { 0,24};break;
  case(directions::left ): src_pt = {24, 0};break;
  case(directions::right): src_pt = {24,24};break;
    }


  int  n = m_animation_counter%4;

    switch(n)
    {
  case(1): src_pt.x += 48*1;break;
  case(3): src_pt.x += 48*2;break;
    }


  gbstd::canvas  src_cv(g_image,src_pt.x,src_pt.y,24,24);

  cv.blend_canvas(src_cv,dst_pt.x-12,dst_pt.y-12,(m_current_position.z+24)*2,-2);
}




}




