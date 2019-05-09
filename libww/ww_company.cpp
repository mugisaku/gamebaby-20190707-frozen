#include"libww/ww_company.hpp"




namespace ww{




company::
company() noexcept
{
}




/*
void
company::
commit_operation_queue() noexcept
{
    switch(o.get_kind())
    {
  case(operation_kind::null):
      break;
  case(operation_kind::level_up):
      break;
  case(operation_kind::level_down):
      break;
  case(operation_kind::hp_up):
      break;
  case(operation_kind::hp_down):
      break;
  case(operation_kind::mp_up):
      break;
  case(operation_kind::mp_down):
      break;
  case(operation_kind::hp_max_up):
      break;
  case(operation_kind::hp_max_down):
      break;
  case(operation_kind::mp_max_up):
      break;
  case(operation_kind::mp_max_down):
      break;
    }
}
*/



void
company::
set_tag(battles::side  side, int  i, gbstd::color  color) noexcept
{
/*
  m_tag = tag(side,i,color);

  int  y = 32+(g_cell_height*i);

  constexpr int  name_w = (gbstd::g_font_width*(8+6));
  constexpr int   bar_w = 256;

  m_front_point  = real_point(-((g_center_space_width/2)+(g_cell_width/2)),y);
  m_back_point   = m_front_point-real_point(g_cell_width,0);
  m_backup_point =  m_back_point-real_point(g_cell_width,0);
  m_name_point   =  m_back_point+real_point(-64,0);

  m_hp_bar.set_position(m_back_point+real_point(-64,gbstd::g_font_height));

    if(m_tag.is_right())
    {
      m_front_point.x  =  -m_front_point.x;
      m_back_point.x   =   -m_back_point.x;
      m_backup_point.x = -m_backup_point.x;

      m_name_point.x = -m_name_point.x-name_w;


      int  x = m_hp_bar.get_position().x;

      m_hp_bar.set_position({-x,y+gbstd::g_font_height});

      m_hp_bar.set_mode(bar::mode::right_to_left);
    }

  else
    {
      m_hp_bar.set_mode(bar::mode::left_to_right);
    }
*/
}




void
company::
reset() noexcept
{
/*
  hp_bar.set_offset(m_offset);

    if()
    {
      m_variable = *m_original;

      m_current_pos = m_backup_point;

      hp_bar.set_mode(m_tag.is_left()? bar::mode::left_to_right:bar::mode::right_to_left);
      hp_bar.set_thickness(8);
      hp_bar.set_color(gbstd::colors::white);
      hp_bar.set_increment(g_bar_length_max,16);

      reset_hp_bar();

      auto  pt = m_variable.is_front_position()? m_front_point
                : m_variable.is_back_position()? m_back_point
                :                                m_backup_point;

      add_absolute_motion(pt,32,1000);

      m_status.set(flags::motion_valid);
    }
*/
}


void
company::
open(const battles::character_head&  h, const battles::character_body&  b) noexcept
{
  m_head = h;

  m_original_body = b;

  m_status.set(flags::opened);
}


void
company::
close() noexcept
{
  m_status.unset(flags::opened);
}




void
company::
rollback() noexcept
{
  m_proof_body = m_product_body;
}


void
company::
commit() noexcept
{
  m_product_body = m_proof_body;
}


void
company::
draw(const gbstd::canvas&  cv, company&  c) noexcept
{
  gbstd::string_form  sf;

  auto  nam_pt = c.m_name_point+c.m_offset;

//  cv.draw_string(gbstd::colors::white,sf("%s",c.m_variable.get_name().data()),nam_pt.x,nam_pt.y);

//  cv.draw_string(gbstd::colors::white,sf("%6d",c.m_variable.get_hp()),nam_pt.x+(gbstd::g_font_width*8),nam_pt.y);
}




}




