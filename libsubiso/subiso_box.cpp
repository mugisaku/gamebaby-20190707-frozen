#include"libsubiso/subiso.hpp"




namespace subiso{




box::
box() noexcept:
m_top_plane(plane::make_top(*this)),
m_side_planes{plane::make_side(*this,0),
              plane::make_side(*this,1),
              plane::make_side(*this,2),
              plane::make_side(*this,3),}
{
}




void
box::
be_water(bool  src) noexcept
{
  m_kind = kind::water;

    if(src){  set_source_flag();}
  else     {unset_source_flag();}
}


void
box::
be_stairs(direction  d) noexcept
{
  m_kind = kind::stairs;

  m_dir = d;
}




}




