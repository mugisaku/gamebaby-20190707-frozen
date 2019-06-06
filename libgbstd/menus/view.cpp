#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{




void
view::
draw_content(const canvas&  cv, const windows::style&  style, view&  v) noexcept
{
  cv.fill(gbstd::colors::blue);

  v.m_table->draw(v.m_offset.x,
                  v.m_offset.y,
                  v.m_width,
                  v.m_height,
                  v.m_callback,cv);
}


view&
view::
assign(table&  tbl) noexcept
{
  m_table = &tbl;

  m_content.set_callback(draw_content,*this);

  m_frame.set_content(&m_content);

  set_width( tbl.get_width() );
  set_height(tbl.get_height());

  m_offset = {0,0};

  m_first_cursor.reset( *this).hide();
  m_second_cursor.reset(*this).hide();

  return *this;
}


view&
view::
set_width(int  w) noexcept
{
  m_width = w;

  m_content.set_width(m_table->get_entry_width()*w);

  return *this;
}


view&
view::
set_height(int  h) noexcept
{
  m_height = h;

  m_content.set_height(m_table->get_entry_height()*h);

  return *this;
}




view&
view::
set_x_offset(int  n) noexcept
{
  m_offset.x = std::max(0,std::min(n,m_width-m_table->get_width()-1));

  return *this;
}


view&
view::
add_x_offset(int  n) noexcept
{
  m_offset.x = std::max(0,std::min(m_offset.x+n,m_width-m_table->get_width()-1));

  return *this;
}


view&
view::
set_y_offset(int  n) noexcept
{
  m_offset.y = std::max(0,std::min(n,m_height-m_table->get_height()-1));

  return *this;
}

view&
view::
add_y_offset(int  n) noexcept
{
  m_offset.y = std::max(0,std::min(m_offset.y+n,m_height-m_table->get_height()-1));

  return *this;
}




void
view::
draw_cursor(const cursor&  cur, const gbstd::canvas&  cv) noexcept
{
    if(cur.is_visible())
    {
      cv.draw_canvas({gbstd::g_misc_image,0,0,24,24},
        m_frame.get_x()-16                                +(m_table->get_entry_width() *cur.get_point().x),
        m_frame.get_y()   +m_frame.get_style().m_top_width+(m_table->get_entry_height()*cur.get_point().y)+4);
    }
}


void
view::
draw(const gbstd::canvas&  cv) noexcept
{
  m_frame.draw(cv);

  draw_cursor( m_first_cursor,cv);
  draw_cursor(m_second_cursor,cv);
}




}}




