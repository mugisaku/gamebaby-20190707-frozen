#include"libww/ww.hpp"




namespace ww{




bool  row::is_left()  const noexcept{return m_force->m_side == side::left;}
bool  row::is_right() const noexcept{return m_force->m_side == side::right;}


void
row::
reset_animation(int  phase) noexcept
{
  m_animation_phase   = phase;
  m_animation_counter = 0;
}


void
row::
reset(force&  force, company*  org) noexcept
{
  m_force = &force;

  m_original = org;

    if(org)
    {
      m_variable = *m_original;
    }


  m_ap = 0;

  m_entry_flag = false;
}


void
row::
render(ww::side  side, gbstd::color  force_color, const gbstd::canvas&  cv) const noexcept
{
    if(!m_original)
    {
      return;
    }


  int  x = m_rendering_pos.x;
  int  y = m_rendering_pos.y;

    if(side == side::left)
    {
      x = cv.get_width()-1-x;
    }


  m_variable.render(force_color,{cv,x,y,g_frame_w,g_frame_h});
}




}




