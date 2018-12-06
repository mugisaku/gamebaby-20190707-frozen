#include"libsubiso/subiso.hpp"




namespace subiso{




void
plane_reference::
reset_top_link(const box_view&  bv) noexcept
{
  m_tops.m_left_up_box  = bv.get_left_up_box();
  m_tops.m_back_up_box  = bv.get_back_up_box();
  m_tops.m_right_up_box = bv.get_right_up_box();
  m_tops.m_front_box    = bv.get_front_box();
  m_tops.m_back_box     = bv.get_back_box();
}


void
plane_reference::
reset_front_link(const box_view&  bv) noexcept
{
  m_fronts.m_left_front_box  = bv.get_left_front_box();
  m_fronts.m_right_front_box = bv.get_right_front_box();
  m_fronts.m_front_down_box  = bv.get_front_down_box();
}


plane_reference&
plane_reference::
assign(direction  dir, plane*  pl) noexcept
{
  m_plane = pl;

    if(pl)
    {
      auto&  box = *m_plane->get_box();

      box_view  bv(box,dir);


      m_left_box  = bv.get_left_box() ;
      m_right_box = bv.get_right_box();

        if(pl->is_top()){  reset_top_link(bv);}
      else              {reset_front_link(bv);}


      m_image_z_base = g_plane_size*(1+box.get_index().z);
    }


  return *this;
}




namespace{
bool
test_top(const box*  dst) noexcept
{
return 0;
}
bool
test_disconnection(const box*  dst, box::kind  k) noexcept
{
return 0;
}
}


int
plane_reference::
get_flags_of_top_plane() const noexcept
{
  auto  box = m_plane->get_box();

  auto  k = box->get_kind();

  int  flags = 0;

/*
        if(is_not_same(m_right_box,k)        || test_top(m_tops.m_right_up_box)){flags |=  right_flag;}
        if(is_not_same(m_left_box,k)         || is_valid(m_tops.m_left_up_box) ){flags |=   left_flag;}
        if(is_not_same(m_tops.m_back_box,k)  || is_valid(m_tops.m_back_up_box) ){flags |=    top_flag;}
        if(is_not_same(m_tops.m_front_box,k)                                   ){flags |= bottom_flag;}
*/

  return flags;
}


int
plane_reference::
get_flags_of_front_plane() const noexcept
{
  auto  box = m_plane->get_box();

  auto  k = box->get_kind();

  int  flags = 0;

/*
       if(is_not_same(m_right_box,k)        || is_valid(m_fronts.m_right_front_box)){flags |=  right_flag;}
        if(is_not_same(m_left_box,k)         || is_valid(m_fronts.m_left_front_box) ){flags |=   left_flag;}
        if(is_not_same(box.get_down_box(),k) || is_valid(m_fronts.m_front_down_box) ){flags |= bottom_flag;}
        if(is_not_same(box.get_up_box(),k)                                          ){flags |=    top_flag;}
*/

  return flags;
}




int
plane_reference::
get_flags() const noexcept
{
  return m_plane->is_top()? get_flags_of_top_plane()
        :                   get_flags_of_front_plane();
}




}




