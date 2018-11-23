#include"libsubiso/subiso.hpp"




namespace subiso{




box::
box() noexcept:
m_top_plane(plane::make_top(*this)),
m_side_planes{plane::make_side(*this),
              plane::make_side(*this),
              plane::make_side(*this),
              plane::make_side(*this),}
{
}




}




