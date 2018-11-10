#include"libsubiso/subiso.hpp"




namespace subiso{




void
space::
reset_distance_all() noexcept
{
    for(auto&  box: m_boxes)
    {
      box.set_distance(0xFFFFFFFF);
    }
}


box*
space::
get_box_pointer(int  x, int  y, int  z) noexcept
{
    if((x >= 0) && (x < m_x_length) &&
       (y >= 0) && (y < m_y_length) &&
       (z >= 0) && (z < m_z_length))
    {
      return &get_box(x,y,z);
    }


  return nullptr;
}


void
space::
resize(int  xl, int  yl, int  zl) noexcept
{
  m_boxes.resize(xl*yl*zl);

  m_x_length = xl;
  m_y_length = yl;
  m_z_length = zl;

    for(int  z = 0;  z < zl;  ++z){
    for(int  y = 0;  y < yl;  ++y){
    for(int  x = 0;  x < xl;  ++x){
      auto&  box = get_box(x,y,z);

      box.m_space = this;

      box.m_index = point3d{x,y,z};

      box.m_kind = box::kind::null;

      box.m_top_plane.m_box   = &box;
      box.m_back_plane.m_box  = &box;
      box.m_front_plane.m_box = &box;
      box.m_left_plane.m_box  = &box;
      box.m_right_plane.m_box = &box;

      box.m_top_plane.m_kind   = plane::kind::top;
      box.m_back_plane.m_kind  = plane::kind::side;
      box.m_front_plane.m_kind = plane::kind::side;
      box.m_left_plane.m_kind  = plane::kind::side;
      box.m_right_plane.m_kind = plane::kind::side;
    }}}
}




}




