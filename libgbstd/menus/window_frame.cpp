#include"libgbstd/menu.hpp"




namespace gbstd{
namespace windows{




int
frame::
get_width() const noexcept
{
  return m_style->m_left_side_width
        +m_content.m_width
        +m_style->m_right_side_width;
}


int
frame::
get_height() const noexcept
{
  return m_style->m_top_width
        +m_content.m_height
        +m_style->m_bottom_width;
}


void
frame::
draw(const gbstd::canvas&  cv) noexcept
{
  int  w = get_width();

    if(m_top)
    {
      gbstd::canvas  tmp_cv(cv,m_position.x,
                               m_position.y,
                               w,
                               m_style->m_top_width);


      m_top(tmp_cv,*m_style);
    }


    if(m_bottom)
    {
      gbstd::canvas  tmp_cv(cv,m_position.x,
                               m_position.y+m_style->m_top_width+m_content.m_height,
                               w,
                               m_style->m_bottom_width);


      m_bottom(tmp_cv,*m_style);
    }


    if(m_left_side)
    {
      gbstd::canvas  tmp_cv(cv,m_position.x,
                               m_position.y+m_style->m_top_width,
                               m_style->m_left_side_width,
                               m_content.m_height);


      m_left_side(tmp_cv,*m_style);
    }


    if(m_right_side)
    {
      gbstd::canvas  tmp_cv(cv,m_position.x+m_style->m_left_side_width+m_content.m_width,
                               m_position.y+m_style->m_top_width,
                               m_style->m_right_side_width,
                               m_content.m_height);


      m_right_side(tmp_cv,*m_style);
    }


    if(m_content.m_callback)
    {
      gbstd::canvas  tmp_cv(cv,m_position.x+m_style->m_left_side_width,
                               m_position.y+m_style->m_top_width,
                               m_content.m_width,
                               m_content.m_height);

      m_content.m_callback(tmp_cv,*m_style,*static_cast<dummy*>(m_content.m_data));
    }
}




}}




