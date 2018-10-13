#include"libgbstd/windows/component.hpp"



namespace gbstd{




void
menu::
resize(int  ncols, int  nrows) noexcept
{
  m_number_of_columns = ncols;
  m_number_of_rows    = nrows;

  m_cursor = item_cursor();

  set_content_width( m_item_size.width *ncols);
  set_content_height(m_item_size.height*nrows);

  request_reform();
}


void
menu::
reset(item_size  itm_sz, item_table_size  itmtbl_sz, int  ncols, int  nrows, callback  cb) noexcept
{
  m_item_size       = itm_sz;
  m_item_table_size = itmtbl_sz;

  m_callback = cb;

  resize(ncols,nrows);
}



void
menu::
move_up() noexcept
{
    if(m_cursor.get_offset().y)
    {
      m_cursor.add_offset({0,-1});

      request_redraw();
    }

  else
    if(m_cursor.get_base().y)
    {
      m_cursor.add_base({0,-1});

      request_redraw();
    }
}


void
menu::
move_down() noexcept
{
    if(m_cursor.get_offset().y < (m_number_of_rows-1))
    {
      m_cursor.add_offset({0,1});

      request_redraw();
    }

  else
    if((m_cursor.get_base().y+m_number_of_rows) < (m_item_table_size.y_length-1))
    {
      m_cursor.add_base({0,1});

      request_redraw();
    }
}


void
menu::
move_left() noexcept
{
    if(m_cursor.get_offset().x)
    {
      m_cursor.add_offset({-1,0});

      request_redraw();
    }

  else
    if(m_cursor.get_base().x)
    {
      m_cursor.add_base({-1,0});

      request_redraw();
    }
}


void
menu::
move_right() noexcept
{
    if(m_cursor.get_offset().x < (m_number_of_columns-1))
    {
      m_cursor.add_offset({1,0});

      request_redraw();
    }

  else
    if((m_cursor.get_base().x+m_number_of_columns) < (m_item_table_size.x_length-1))
    {
      m_cursor.add_base({1,0});

      request_redraw();
    }
}


void
menu::
do_on_mouse_act(point  mouse_pos) noexcept
{
  int  x = mouse_pos.x/m_item_size.width ;
  int  y = mouse_pos.y/m_item_size.height;

  point  current(x,y);

    if(m_cursor.get_offset() != current)
    {
      call(menu_event::kind::leave);

      m_cursor.set_offset(current);

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
menu::
render(const canvas&  cv) noexcept
{
  int  item_w = m_item_size.width ;
  int  item_h = m_item_size.height;

  auto  bas = m_cursor.get_base();

  render_background(cv);

    for(int  y = 0;  y < m_number_of_rows   ;  ++y){
    for(int  x = 0;  x < m_number_of_columns;  ++x){
      canvas  cocv(cv,item_w*x,item_h*y,
                      item_w  ,item_h  );

      render_item(bas+point(x,y),cocv);
    }}
}




}




