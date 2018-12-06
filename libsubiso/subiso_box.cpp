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
  m_top_edge_flags    = 0;
  m_middle_edge_flags = 0;
  m_bottom_edge_flags = 0;
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




