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
test_top_edge(box*  dst, direction  dir, int  flag) noexcept
{
    if(!dst)
    {
      return false;
    }


  box_view  bv(*dst,dir);

  return bv.test_top_edge(flag);
}
bool
test_middle_edge(box*  dst, direction  dir, int  flag) noexcept
{
    if(!dst)
    {
      return false;
    }


  box_view  bv(*dst,dir);

  return bv.test_middle_edge(flag);
}
bool
test_bottom_edge(box*  dst, direction  dir, int  flag) noexcept
{
    if(!dst)
    {
      return false;
    }


  box_view  bv(*dst,dir);

  return bv.test_bottom_edge(flag);
}
bool
is_not_same(const box*  dst, box::kind  k) noexcept
{
  return dst && (dst->get_kind() != k);
}
bool
is_no_water_filled(const box*  dst) noexcept
{
  return dst && !dst->test_water_filled_flag();
}
}


int
plane_reference::
get_flags_of_top_plane(direction  dir) const noexcept
{
  auto  box = m_plane->get_box();

  auto  k = box->get_kind();

  int  flags = 0;

    if(box->test_water_filled_flag())
    {
        if(test_bottom_edge(m_tops.m_left_up_box,dir,edge_flags::right) ||
           is_no_water_filled(m_left_box))
        {
          flags |= left_flag;
        }


        if(test_bottom_edge(m_tops.m_right_up_box,dir,edge_flags::left) ||
           is_no_water_filled(m_right_box))
        {
          flags |= right_flag;
        }


        if(test_bottom_edge(m_tops.m_back_up_box,dir,edge_flags::front) ||
           is_no_water_filled(m_tops.m_back_box))
        {
          flags |= top_flag;
        }


        if(is_no_water_filled(m_tops.m_front_box))
        {
          flags |= bottom_flag;
        }
    }

  else
    if(box->is_earth())
    {
        if(test_bottom_edge(m_tops.m_left_up_box,dir,edge_flags::right) ||
           is_not_same(m_left_box,k))
        {
          flags |= left_flag;
        }


        if(test_bottom_edge(m_tops.m_right_up_box,dir,edge_flags::left) ||
           is_not_same(m_right_box,k))
        {
          flags |= right_flag;
        }


        if(test_bottom_edge(m_tops.m_back_up_box,dir,edge_flags::front) ||
           is_not_same(m_tops.m_back_box,k))
        {
          flags |= top_flag;
        }


        if(is_not_same(m_tops.m_front_box,k))
        {
          flags |= bottom_flag;
        }
    }


  return flags;
}


int
plane_reference::
get_flags_of_front_plane(direction  dir) const noexcept
{
  auto  box = m_plane->get_box();

  auto  k = box->get_kind();

  int  flags = 0;

    if(box->test_water_filled_flag())
    {
        if(test_middle_edge(m_fronts.m_left_front_box,dir,edge_flags::right_front) ||
           is_no_water_filled(m_left_box))
        {
          flags |= left_flag;
        }


        if(test_middle_edge(m_fronts.m_right_front_box,dir,edge_flags::left_front) ||
           is_no_water_filled(m_right_box))
        {
          flags |= right_flag;
        }


        if(test_top_edge(m_fronts.m_front_down_box,dir,edge_flags::back) ||
           is_no_water_filled(box->get_down_box()))
        {
          flags |= bottom_flag;
        }


        if(is_no_water_filled(box->get_up_box()))
        {
          flags |= top_flag;
        }
    }

  else
    if(box->is_earth())
    {
        if(test_middle_edge(m_fronts.m_left_front_box,dir,edge_flags::right_front) ||
           is_not_same(m_left_box,k))
        {
          flags |= left_flag;
        }


        if(test_middle_edge(m_fronts.m_right_front_box,dir,edge_flags::left_front) ||
           is_not_same(m_right_box,k))
        {
          flags |= right_flag;
        }


        if(test_top_edge(m_fronts.m_front_down_box,dir,edge_flags::back) ||
           is_not_same(box->get_down_box(),k))
        {
          flags |= bottom_flag;
        }


        if(is_not_same(box->get_up_box(),k))
        {
          flags |= top_flag;
        }
    }


  return flags;
}




int
plane_reference::
get_flags(direction  dir) const noexcept
{
  return m_plane->is_top()? get_flags_of_top_plane(  dir)
        :                   get_flags_of_front_plane(dir);
}




}




