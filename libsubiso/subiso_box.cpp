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
update_edge_flags() noexcept
{
    if(test_water_filled_flag() || is_earth())
    {
      m_top_edge_flags    = 0xF;
      m_middle_edge_flags = 0xF;
      m_bottom_edge_flags = 0xF;
    }

  else
    if(is_stairs())
    {
      m_top_edge_flags    = edge_flags::back;
      m_middle_edge_flags = edge_flags::left_back|edge_flags::right_back;
      m_bottom_edge_flags = edge_flags::left|edge_flags::right|edge_flags::back;

      m_top_edge_flags    = get_shifted_flags(   m_top_edge_flags,m_dir);
      m_middle_edge_flags = get_shifted_flags(m_middle_edge_flags,m_dir);
      m_bottom_edge_flags = get_shifted_flags(m_bottom_edge_flags,m_dir);
    }
}


void
box::
update() noexcept
{
  update_edge_flags();

  m_ground_line = g_plane_size*m_index.z;

    if(test_water_filled_flag() || is_stairs())
    {
      m_ground_line += (g_plane_size/2);
    }
}




bool
box::
is_surrounded_by_earth() const noexcept
{
  return get_middle_box(0) && get_middle_box(0)->is_earth() &&
         get_middle_box(2) && get_middle_box(2)->is_earth() &&
         get_middle_box(4) && get_middle_box(4)->is_earth() &&
         get_middle_box(6) && get_middle_box(6)->is_earth();
}


bool
box::
is_surrounded_by_water() const noexcept
{
  return get_middle_box(0) && get_middle_box(0)->test_water_filled_flag() &&
         get_middle_box(2) && get_middle_box(2)->test_water_filled_flag() &&
         get_middle_box(4) && get_middle_box(4)->test_water_filled_flag() &&
         get_middle_box(6) && get_middle_box(6)->test_water_filled_flag();
}


bool
box::
is_contacted_water() const noexcept
{
  return (get_middle_box(0) && get_middle_box(0)->test_water_filled_flag()) ||
         (get_middle_box(2) && get_middle_box(2)->test_water_filled_flag()) ||
         (get_middle_box(4) && get_middle_box(4)->test_water_filled_flag()) ||
         (get_middle_box(6) && get_middle_box(6)->test_water_filled_flag());
}




void
box::
flow_water() noexcept
{
  get_space()->clear_search_flags();

  std::vector<box*>  buf;

  buf.emplace_back(this);

  int  i = 0;

    while(i < buf.size())
    {
      auto  b = buf[i++];

        if(b && !b->is_earth() && !b->is_checked())
        {
          b->check();

            if(!b->test_water_filled_flag())
            {
                if(b->is_contacted_water())
                {
                  b->set_water_filled_flag();
                }


              buf.emplace_back(b->get_middle_box(0));
              buf.emplace_back(b->get_middle_box(2));
              buf.emplace_back(b->get_middle_box(4));
              buf.emplace_back(b->get_middle_box(6));
            }
        }
    }
}


void
box::
be_stairs(direction  d) noexcept
{
  m_kind = kind::stairs;

  m_dir = d;
}




}




