#include"libgbstd/windows/component.hpp"



namespace gbstd{




void
menu::
process_before_reform() noexcept
{
  set_content_width( m_item_size.width *m_number_of_columns);
  set_content_height(m_item_size.height*m_number_of_rows   );
}


void
menu::
revise_cursor() noexcept
{
}


void
menu::
set_item_size(item_size  itm_sz) noexcept
{
  m_item_size = itm_sz;

  request_reform();
}


void
menu::
set_table_size(table_size  tbl_sz) noexcept
{
  m_table_size = tbl_sz;

  revise_cursor();
}


void
menu::
resize(int  ncols, int  nrows) noexcept
{
  m_number_of_columns = ncols;
  m_number_of_rows    = nrows;

  revise_cursor();

  request_reform();
}




void
menu::
move_base(int  x, int  y) noexcept
{
  int  x_res = (m_cursor.get_base().x+x);
  int  y_res = (m_cursor.get_base().y+y);

    if(x_res < 0)
    {
      x_res = 0;
    }

  else
    if((x_res+m_number_of_columns) >= m_table_size.x_length)
    {
      x_res = (m_table_size.x_length-m_number_of_columns);
    }


    if(y_res < 0)
    {
      y_res = 0;
    }

  else
    if((y_res+m_number_of_rows) >= m_table_size.y_length)
    {
      y_res = (m_table_size.y_length-m_number_of_rows);
    }


  m_cursor.set_base({x_res,y_res});

  request_redraw();
}


bool
menu::
move_x_offset(int  n) noexcept
{
  bool  b = false;

  int  x_res = (m_cursor.get_offset().x+n);

    if(x_res < 0)
    {
      x_res = 0;

      b = true;
    }

  else
    if(x_res >= m_number_of_columns)
    {
      x_res = m_number_of_columns-1;

      b = true;
    }


  m_cursor.set_offset({x_res,m_cursor.get_offset().y});

  request_redraw();

  return b;
}


bool
menu::
move_y_offset(int  n) noexcept
{
  bool  b = false;

  int  y_res = (m_cursor.get_offset().y+n);

    if(y_res < 0)
    {
      y_res = 0;

      b = true;
    }

  else
    if(y_res >= m_number_of_rows)
    {
      y_res = m_number_of_rows-1;

      b = true;
    }


  m_cursor.set_offset({m_cursor.get_offset().x,y_res});

  request_redraw();

  return b;
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




