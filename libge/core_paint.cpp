#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


void
core_paint::
reset(const gbstd::canvas&  cv) noexcept
{
    if(cv)
    {
      cancel_drawing();
      cancel_select();
    }


  m_drawing_is_fixed = true;

  m_recorder.clear();

  m_canvas = cv;
}




void
core_paint::
cancel_drawing() noexcept
{
  m_pointing_count = 0;

    if(!m_drawing_is_fixed)
    {
        if((m_mode == mode::paste) ||
           (m_mode == mode::layer))
        {
          m_recorder.rollback(m_canvas);
        }


      m_drawing_is_fixed = true;
    }
}


void
core_paint::
cancel_select() noexcept
{
  m_operation_rect.x = 0;
  m_operation_rect.y = 0;
  m_operation_rect.w = m_canvas.get_width() ;
  m_operation_rect.h = m_canvas.get_height();
}


void
core_paint::
modify_dot(gbstd::color  color, gbstd::point  pt) noexcept
{
  auto&  pix = *m_canvas.get_pixel_pointer(pt.x,pt.y);

    if(pix.color != color)
    {
      m_recorder.put(pix.color,pt.x,pt.y);

      pix.color = color;
    }
}




void
core_paint::
take_copy() noexcept
{
  m_clipped_image.resize(m_operation_rect.w,m_operation_rect.h);

    for(int  y = 0;  y < m_operation_rect.h;  ++y){
    for(int  x = 0;  x < m_operation_rect.w;  ++x){
      auto  pix = *m_canvas.get_pixel_pointer(m_operation_rect.x+x,m_operation_rect.y+y);

      *m_clipped_image.get_pixel_pointer(x,y) = pix;
    }}
}


void
core_paint::
undo() noexcept
{
    if(m_recorder.rollback(m_canvas))
    {
//
    }
}




void
core_paint::
try_to_push_solid_record() noexcept
{
    if(m_recorder.push(true))
    {
      m_drawing_is_fixed = false;
    }
}


void
core_paint::
try_to_push_nonsolid_record() noexcept
{
    if(m_drawing_is_fixed)
    {
      m_recorder.push(false);
    }

  else
    {
      m_drawing_is_fixed = true;
    }
}




gbstd::image
core_paint::
get_temporary_image() noexcept
{
  int  w = m_canvas.get_width() ;
  int  h = m_canvas.get_height();

  gbstd::image  img(w,h);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  pix = *m_canvas.get_pixel_pointer(x,y);

      *img.get_pixel_pointer(x,y) = pix;
    }}


  return std::move(img);
}




bool
core_paint::
operator()() noexcept
{
  bool  dirty_flag = false;

    switch(m_mode)
    {
  case(mode::draw_dot):
        if(gbstd::g_input.test_mouse_left())
        {
          modify_dot(m_drawing_color,m_drawing_point);

          dirty_flag = true;
        }

      else
        if(gbstd::g_input.test_mouse_right())
        {
          modify_dot(gbstd::color(),m_drawing_point);

          dirty_flag = true;
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
                }


              draw_line(m_drawing_color);

              dirty_flag = true;
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);
                }


              draw_line(color());

              dirty_flag = true;
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
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::draw_rectangle):
        if(m_pointing_count)
        {
          gbstd::rectangle  rect(m_saved_drawing_point,m_drawing_point);

            if(gbstd::g_input.test_mouse_left())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);
                }


              draw_rect(m_drawing_color,rect);

              dirty_flag = true;
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);
                }


              draw_rect(gbstd::color(),rect);

              dirty_flag = true;
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
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_rectangle):
        if(m_pointing_count)
        {
          gbstd::rectangle  rect(m_saved_drawing_point,m_drawing_point);

            if(gbstd::g_input.test_mouse_left())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);
                }


              fill_rect(m_drawing_color,rect);

              dirty_flag = true;
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
                if(!m_drawing_is_fixed)
                {
                  m_recorder.rollback(m_canvas);
                }


              fill_rect(gbstd::color(),rect);

              dirty_flag = true;
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
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_area):
        if(gbstd::g_input.test_mouse_left())
        {
          fill_area(m_drawing_color);

          dirty_flag = true;
        }

      else
        if(gbstd::g_input.test_mouse_right())
        {
          fill_area(gbstd::color());

          dirty_flag = true;
        }
      break;
  case(mode::select):
        if(m_pointing_count)
        {
            if(gbstd::g_input.test_mouse_left())
            {
              m_operation_rect = gbstd::rectangle(m_saved_drawing_point,m_drawing_point);

              dirty_flag = true;
            }

          else
            {
              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::g_input.test_mouse_left())
            {
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }

          else
            if(gbstd::g_input.test_mouse_right())
            {
              m_operation_rect = gbstd::rectangle(0,0,m_canvas.get_width(),m_canvas.get_height());

              dirty_flag = true;
            }
        }
      break;
  case(mode::paste):
        if(gbstd::g_input.test_mouse_left())
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_saved_drawing_point != m_drawing_point)
        {
            if(!m_drawing_is_fixed)
            {
              m_recorder.rollback(m_canvas);
            }


          paste(false);

          dirty_flag = true;
        }


      m_saved_drawing_point = m_drawing_point;
      break;
  case(mode::layer):
        if(gbstd::g_input.test_mouse_left())
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_saved_drawing_point != m_drawing_point)
        {
            if(!m_drawing_is_fixed)
            {
              m_recorder.rollback(m_canvas);
            }


          paste(true);

          dirty_flag = true;
        }


      m_saved_drawing_point = m_drawing_point;
      break;
    }


  return dirty_flag;
}




}




