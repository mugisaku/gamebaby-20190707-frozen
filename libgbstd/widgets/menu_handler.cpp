#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




menu_handler::
menu_handler(operating_node&  root) noexcept
{
}




void
menu_handler::
process_before_reform() noexcept
{
  set_content_width( m_view->get_width() *m_view->get_table().get_entry_width() );
  set_content_height(m_view->get_height()*m_view->get_table().get_entry_height());
}




menu_handler&
menu_handler::
set_view(menus::view&  v) noexcept
{
  static const gbstd::windows::style  style = {0,0,0,0};

  m_view = &v;

  auto&  frm = v.get_window_frame();

  frm.set_assembler(nullptr);
  frm.set_style(style);

  request_reform();

  return *this;
}


void
menu_handler::
on_mouse_act(point  mouse_pos) noexcept
{
  auto&  tbl = m_view->get_table();

  int  x = mouse_pos.x/tbl.get_entry_width() ;
  int  y = mouse_pos.y/tbl.get_entry_height();

  point  current(x,y);

  auto&  cursor = m_view->get_first_cursor();

    if(cursor.get_position() != current)
    {
      call(menu_event::kind::leave);

      cursor.set_position(current);

      call(menu_event::kind::enter);
    }


    if(g_modified_input.test_mouse_left())
    {
        if(g_input.test_mouse_left())
        {
          call(menu_event::kind::press);
        }

      else
        {
          call(menu_event::kind::release);
        }


      request_redraw();
    }
}


void
menu_handler::
render(const canvas&  cv) noexcept
{
  m_view->draw(cv);
}




}}




