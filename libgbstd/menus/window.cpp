#include"libgbstd/menu.hpp"




namespace gbstd{
namespace windows{




style
g_default_style = {8,8,8,8};


namespace{
void
top(const canvas&  cv, const windows::style&  style) noexcept
{
  cv.fill(gbstd::colors::red);
}
void
left(const canvas&  cv, const windows::style&  style) noexcept
{
  cv.fill(gbstd::colors::gray);
}
void
right(const canvas&  cv, const windows::style&  style) noexcept
{
  cv.fill(gbstd::colors::green);
}
void
bottom(const canvas&  cv, const windows::style&  style) noexcept
{
  cv.fill(gbstd::colors::yellow);
}
void
background(const canvas&  cv, const windows::style&  style) noexcept
{
  cv.fill(gbstd::colors::blue);
}
}


frame_assembler
g_default_frame_assembler =
{
  top,bottom,left,right
};


int
frame::
get_width() const noexcept
{
  return m_style_pointer->m_left_side_width
        +(m_content_pointer? m_content_pointer->m_width:0)
        +m_style_pointer->m_right_side_width;
}


int
frame::
get_height() const noexcept
{
  return m_style_pointer->m_top_width
        +(m_content_pointer? m_content_pointer->m_height:0)
        +m_style_pointer->m_bottom_width;
}


void
frame::
draw(const gbstd::canvas&  cv) noexcept
{
  static const content  null;

  const content*  cont_ptr = m_content_pointer? m_content_pointer:&null;

  int  w = get_width();

    if(m_assembler_pointer)
    {
      auto&  as = *m_assembler_pointer;

        if(as.m_top)
        {
          gbstd::canvas  tmp_cv(cv,m_position.x,
                                   m_position.y,
                                   w,
                                   m_style_pointer->m_top_width);


          as.m_top(tmp_cv,*m_style_pointer);
        }


        if(as.m_bottom)
        {
          gbstd::canvas  tmp_cv(cv,m_position.x,
                                   m_position.y+m_style_pointer->m_top_width+cont_ptr->m_height,
                                   w,
                                   m_style_pointer->m_bottom_width);


          as.m_bottom(tmp_cv,*m_style_pointer);
        }


        if(as.m_left_side)
        {
          gbstd::canvas  tmp_cv(cv,m_position.x,
                                   m_position.y+m_style_pointer->m_top_width,
                                   m_style_pointer->m_left_side_width,
                                   cont_ptr->m_height);


          as.m_left_side(tmp_cv,*m_style_pointer);
        }
    

        if(as.m_right_side)
        {
          gbstd::canvas  tmp_cv(cv,m_position.x+m_style_pointer->m_left_side_width+cont_ptr->m_width,
                                   m_position.y+m_style_pointer->m_top_width,
                                   m_style_pointer->m_right_side_width,
                                   cont_ptr->m_height);


          as.m_right_side(tmp_cv,*m_style_pointer);
        }
    

        if(as.m_background)
        {
          gbstd::canvas  tmp_cv(cv,m_position.x+m_style_pointer->m_left_side_width,
                                   m_position.y+m_style_pointer->m_top_width,
                                   cont_ptr->m_width,
                                   cont_ptr->m_height);


          as.m_background(tmp_cv,*m_style_pointer);
        }
    }


    if(m_content_pointer)
    {
      m_content_pointer->draw(cv,*m_style_pointer,m_position.x,m_position.y);
    }
}


void
content::
draw(const canvas&  cv, const style&  s, int  x, int  y) noexcept
{
    if(m_callback)
    {
      gbstd::canvas  tmp_cv(cv,x+s.m_left_side_width,
                               y+s.m_top_width,
                               m_width,
                               m_height);

      m_callback(tmp_cv,s,*static_cast<dummy*>(m_data));
    }
}




}}




