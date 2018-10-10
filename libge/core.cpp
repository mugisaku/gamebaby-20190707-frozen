#include"libge/ge.hpp"



core::
core() noexcept
{
}


core::
core(const gbstd::image&  img, int  w, int  h, void  (*callback)(core_event  evt)) noexcept:
m_callback(callback)
{
  set_image(img,w,h);
}




void
core::
set_cursor_offset(int  x, int  y) noexcept
{
  m_cursor_offset = {x,y};

  m_canvas = gbstd::canvas(*m_image,x,y,m_editing_width,m_editing_height);

  request_redraw();
}


void
core::
set_editing_size(int  w, int  h) noexcept
{
    if(m_image)
    {
      cancel_select();
      cancel_drawing();
    }


  m_editing_width  = w;
  m_editing_height = h;

  m_drawing_is_fixed = true;

  m_recorder.clear();

  reset();
}


void
core::
set_pixel_size(int  n) noexcept
{
  m_pixel_size = n;

  reset();
}


void
core::
set_grid() noexcept
{
  m_grid = true;

  request_redraw();
}


void
core::
unset_grid() noexcept
{
  m_grid = false;

  request_redraw();
}




void
core::
reset() noexcept
{
  m_operation_rect = gbstd::rectangle(0,0,m_editing_width,m_editing_height);

  set_content_width( m_pixel_size*m_editing_width );
  set_content_height(m_pixel_size*m_editing_height);

  request_reform();
}


void
core::
set_image(const gbstd::image&  img, int  w, int  h) noexcept
{
  set_editing_size(w,h);

  m_image = &img;
  m_canvas = gbstd::canvas(img,0,0,w,h);
}


void
core::
modify_dot(gbstd::color  new_color, int  x, int  y) noexcept
{
  auto&  pix = *m_canvas.get_pixel_pointer(x,y);

    if(pix.color != new_color)
    {
      m_recorder.put(pix.color,m_cursor_offset.x+x,
                               m_cursor_offset.y+y);

      pix.color = new_color;


      request_redraw();
    }
}




void
core::
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

      request_redraw();
    }
}




void
core::
try_to_push_solid_record() noexcept
{
    if(m_recorder.push(true))
    {
      m_drawing_is_fixed = false;

      request_redraw();

        if(m_callback)
        {
          m_callback({*this,core_event::kind::image_modified});
        }
    }
}


void
core::
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




void
core::
cancel_select() noexcept
{
  m_operation_rect.x = 0;
  m_operation_rect.y = 0;
  m_operation_rect.w = m_editing_width ;
  m_operation_rect.h = m_editing_height;

  request_redraw();
}


void
core::
do_on_mouse_leave() noexcept
{
  cancel_drawing();
}


void
core::
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
core::
undo() noexcept
{
    if(m_recorder.rollback(m_canvas))
    {
      request_redraw();

        if(m_callback)
        {
          m_callback({*this,core_event::kind::image_modified});
        }
    }
}


gbstd::image
core::
get_temporary_image() const noexcept
{
  gbstd::image  img(m_editing_width,m_editing_height);

    for(int  y = 0;  y < m_editing_height;  ++y){
    for(int  x = 0;  x < m_editing_width ;  ++x){
      auto  pix = *m_canvas.get_pixel_pointer(x,y);

      *img.get_pixel_pointer(x,y) = pix;
    }}


  return std::move(img);
}


void
core::
render(const gbstd::canvas&  cv) noexcept
{
  const int  w = m_editing_width ;
  const int  h = m_editing_height;

//  render_background(cur);

    if(m_underlay_point_list)
    {
        for(auto&  pt: *m_underlay_point_list)
        {
            for(int  y = 0;  y < h;  ++y){
            for(int  x = 0;  x < w;  ++x){
              auto&  pix = *m_image->get_pixel_pointer(pt.x+x,pt.y+y);

                if(pix.color)
                {
                  cv.fill_rectangle(pix.color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
                }
            }}
        }
    }


    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  pix = *m_canvas.get_pixel_pointer(x,y);

        if(pix.color)
        {
          cv.fill_rectangle(pix.color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
        }
    }}


    if(m_grid)
    {
        for(int  y = 0;  y < h;  ++y)
        {
          auto  color = (y%8)? gbstd::colors::gray
                             : gbstd::colors::light_gray;

          cv.draw_hline(color,0,m_pixel_size*y,get_content_width());
        }


        for(int  x = 0;  x < w;  ++x)
        {
          auto  color = (x%8)? gbstd::colors::gray
                             : gbstd::colors::light_gray;

          cv.draw_vline(color,m_pixel_size*x,0,get_content_height());
        }


      cv.draw_hline(gbstd::colors::white,0,m_pixel_size*(h/2)  ,get_content_width() );
      cv.draw_vline(gbstd::colors::white,  m_pixel_size*(w/2),0,get_content_height());
    }


  cv.draw_double_rectangle(gbstd::colors::white,gbstd::colors::black,
    m_pixel_size*m_operation_rect.x,
    m_pixel_size*m_operation_rect.y,
    m_pixel_size*m_operation_rect.w,
    m_pixel_size*m_operation_rect.h
  );
}




