#include"libww/ww_entry.hpp"




namespace ww{




void
entry::
render(gbstd::color  force_color, const gbstd::canvas&  cv) const noexcept
{
    if(m_hp){cv.fill_rectangle(force_color,0,0,16,16);}
  else      {cv.draw_rectangle(force_color,0,0,16,16);}
}




}




