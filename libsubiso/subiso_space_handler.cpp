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

  return *this;
}




}




