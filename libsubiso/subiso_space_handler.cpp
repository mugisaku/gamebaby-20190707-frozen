#include"libsubiso/subiso.hpp"




namespace subiso{




space_handler&
space_handler::
assign(space&  sp) noexcept
{
  m_map_table[directions::front].assign(space_view(sp,directions::front));
  m_map_table[directions::left ].assign(space_view(sp,directions::left ));
  m_map_table[directions::right].assign(space_view(sp,directions::right));
  m_map_table[directions::back ].assign(space_view(sp,directions::back ));

  m_current_map = m_map_table;

  set_view_size(g_plane_size*std::min(sp.get_x_length(),sp.get_y_length()),
                g_plane_size*        (sp.get_y_length()+sp.get_z_length()));

  return *this;
}




void
space_handler::
set_view_size(int  w, int  h) noexcept
{
  m_view_width  = w;
  m_view_height = h;
}


void
space_handler::
render(const gbstd::canvas&  cv) noexcept
{
  gbstd::canvas  cocv(cv,0,0,m_view_width,m_view_height);

  int  x = m_view_offset.x                ;
  int  y = m_view_offset.y+m_view_offset.z;

  m_current_map->render({x,y},cocv);
}


void
space_handler::
render(direction  dir, const gbstd::canvas&  cv) noexcept
{
  gbstd::canvas  cocv(cv,0,0,m_view_width,m_view_height);

  int  x = m_view_offset.x                ;
  int  y = m_view_offset.y+m_view_offset.z;

  m_map_table[dir].render({x,y},cocv);
}




}




