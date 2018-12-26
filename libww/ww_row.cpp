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
  m_white_flag = false;
}


void
row::
render(const gbstd::canvas&  cv) const noexcept
{
    if(!m_original)
    {
      return;
    }


  int  bas_x = m_base_pos.x;
  int  bas_y = m_base_pos.y+16;

  int  cur_x = m_current_pos.x;
  int  cur_y = m_current_pos.y;

    if(is_left())
    {
      bas_x = cv.get_width()-1-bas_x;
      cur_x = cv.get_width()-1-cur_x;
    }


    if(!m_blink_context.is_valid() || m_blink_context.is_visible())
    {
      m_variable.render(m_white_flag? gbstd::colors::white:m_force->m_color,{cv,cur_x,cur_y,g_frame_w,g_frame_h});
    }


  gbstd::string_form  sf;

  cv.draw_string(gbstd::colors::white,sf("%6d",m_variable.m_hp),bas_x,bas_y);
}




}




