#include"libww/ww.hpp"




namespace ww{




void
company::
render(gbstd::color  force_color, const gbstd::canvas&  cv) const noexcept
{
    if(m_number_of_soldiers){cv.fill_rectangle(force_color,0,0,16,16);}
  else                      {cv.draw_rectangle(force_color,0,0,16,16);}

  gbstd::string_form  sf;

  cv.draw_string(gbstd::colors::white,sf("%6d",m_number_of_soldiers),0,0);
}




}




