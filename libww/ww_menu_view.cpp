#include"libww/ww_menu.hpp"
#include"libww/ww_context.hpp"




namespace ww{
namespace menus{




constexpr int  g_margin = 8;


view&
view::
assign(table&  tbl) noexcept
{
  m_table = &tbl;

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

  m_window.w = (m_table->get_entry_width()*w)+(g_margin*2);

  return *this;
}


view&
view::
set_height(int  h) noexcept
{
  m_height = h;

  m_window.h = (m_table->get_entry_height()*h)+(g_margin*2);

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
        m_window.x-12      +(m_table->get_entry_width() *cur.get_point().x),
        m_window.y+g_margin+(m_table->get_entry_height()*cur.get_point().y)+4);
    }
}


void
view::
draw(const gbstd::canvas&  cv) noexcept
{
  cv.fill_rectangle(m_window.m_color,
    m_window.x+1,
    m_window.y+1,
    m_window.w-2,
    m_window.h-2);


  cv.draw_rectangle(gbstd::colors::white,
    m_window.x,
    m_window.y,
    m_window.w,
    m_window.h);


  gbstd::canvas  dst_cv(cv,m_window.x+g_margin,
                           m_window.y+g_margin,
                           cv.get_width() -(g_margin*2),
                           cv.get_height()-(g_margin*2));

  m_table->draw(m_offset.x,m_offset.y,m_width,m_height,m_callback,dst_cv);

  draw_cursor( m_first_cursor,cv);
  draw_cursor(m_second_cursor,cv);
}




}}




