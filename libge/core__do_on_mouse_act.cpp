#include"libge/ge.hpp"




void
core::
do_on_mouse_act(gbstd::point  mouse_pos) noexcept
{
  int  x = mouse_pos.x/m_pixel_size;
  int  y = mouse_pos.y/m_pixel_size;

  bool  cursor_moved = (m_current_point != gbstd::point(x,y));

  m_current_point.x = x;
  m_current_point.y = y;

    if(cursor_moved && m_callback)
    {
      m_callback({*this,core_event::kind::painting_cursor_moved});
    }


    switch(m_mode)
    {
  case(mode::draw_dot):
        if(gbstd::g_input.test_mouse_left())
        {
          modify_dot(m_drawing_color,x,y);

            if(m_callback)
            {
              m_callback({*this,core_event::kind::image_modified});
            }
        }

      else
        if(gbstd::g_input.test_mouse_right())
        {
          modify_dot(gbstd::color(),x,y);

            if(m_callback)
            {
              m_callback({*this,core_event::kind::image_modified});
            }
        }
      break;
  case(mode::draw_line):
        if(m_pointing_count)
        {
            if(gbstd::g_input.test_mouse_left())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);

                  request_redraw();
                }


              draw_line(m_drawing_color,m_a_point,gbstd::point(x,y));
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);

                  request_redraw();
                }


              draw_line(gbstd::color(),m_a_point,gbstd::point(x,y));
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::g_input.test_mouse_left() ||
               gbstd::g_input.test_mouse_right())
            {
              m_a_point = gbstd::point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::draw_rectangle):
        if(m_pointing_count)
        {
            if(gbstd::g_input.test_mouse_left())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);

                  request_redraw();
                }


              draw_rect(m_drawing_color,gbstd::rectangle(m_a_point,gbstd::point(x,y)));
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);

                  request_redraw();
                }


              draw_rect(gbstd::color(),gbstd::rectangle(m_a_point,gbstd::point(x,y)));
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::g_input.test_mouse_left() ||
               gbstd::g_input.test_mouse_right())
            {
              m_a_point = gbstd::point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_rectangle):
        if(m_pointing_count)
        {
            if(gbstd::g_input.test_mouse_left())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);

                  request_redraw();
                }


              fill_rect(m_drawing_color,gbstd::rectangle(m_a_point,gbstd::point(x,y)));
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);

                  request_redraw();
                }


              fill_rect(gbstd::color(),gbstd::rectangle(m_a_point,gbstd::point(x,y)));
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::g_input.test_mouse_left() ||
               gbstd::g_input.test_mouse_right())
            {
              m_a_point = gbstd::point(x,y);

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_area):
        if(gbstd::g_input.test_mouse_left())
        {
          fill_area(m_drawing_color,gbstd::point(x,y));
        }

      else
        if(gbstd::g_input.test_mouse_right())
        {
          fill_area(gbstd::color(),gbstd::point(x,y));
        }
      break;
  case(mode::select):
        if(m_pointing_count)
        {
            if(gbstd::g_input.test_mouse_left())
            {
              m_operation_rect = gbstd::rectangle(m_a_point,gbstd::point(x,y));

              request_redraw();
            }

          else
            {
              m_pointing_count = 0;

              request_redraw();
            }
        }

      else
        {
            if(gbstd::g_input.test_mouse_left())
            {
              m_a_point = gbstd::point(x,y);

              m_pointing_count = 1;
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
              m_operation_rect = gbstd::rectangle(0,0,m_editing_width,m_editing_height);

              request_redraw();
            }
        }
      break;
  case(mode::paste):
      m_a_point = gbstd::point(x,y);

        if(gbstd::g_input.test_mouse_left())
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_a_point != m_b_point)
        {
            if(!m_drawing_is_fixed)
            {
              m_recorder.rollback(m_canvas);

              request_redraw();
            }


          m_b_point = m_a_point;

          paste(m_a_point,false);
        }
      break;
  case(mode::layer):
      m_a_point = gbstd::point(x,y);

        if(gbstd::g_input.test_mouse_left())
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_a_point != m_b_point)
        {
            if(!m_drawing_is_fixed)
            {
              m_recorder.rollback(m_canvas);

              request_redraw();
            }


          m_b_point = m_a_point;

          paste(m_a_point,true);
        }
      break;
    }
}




