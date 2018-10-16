#include"libge/ge.hpp"




namespace ge{


core::
core(void  (*callback)(core_event  evt)) noexcept:
m_callback(callback)
{
  m_bg_style.first_color  = gbstd::color(0,0,7);
  m_bg_style.second_color = gbstd::color(2,2,5);
}


core::
core(const gbstd::canvas&  cv, void  (*callback)(core_event  evt)) noexcept:
m_callback(callback)
{
  m_bg_style.first_color  = gbstd::color(0,0,7);
  m_bg_style.second_color = gbstd::color(2,2,5);

  set_canvas(cv);
}




void
core::
set_canvas(const gbstd::canvas&  cv) noexcept
{
    if(m_canvas)
    {
      cancel_select();
      cancel_drawing();

      m_drawing_is_fixed = true;

      m_recorder.clear();
    }


  m_canvas = cv;

  reset();

  request_redraw();
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
  int  w = m_canvas.get_width();
  int  h = m_canvas.get_height();

  m_operation_rect = gbstd::rectangle(0,0,w,h);

  set_content_width( m_pixel_size*w);
  set_content_height(m_pixel_size*h);

  request_reform();
}


void
core::
modify_dot(gbstd::color  new_color, int  x, int  y) noexcept
{
  auto&  pix = *m_canvas.get_pixel_pointer(x,y);

    if(pix.color != new_color)
    {
      m_recorder.put(pix.color,x,y);

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
  m_operation_rect.w = m_canvas.get_width();
  m_operation_rect.h = m_canvas.get_height();

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


void
core::
set_background_style(background_style  bgst) noexcept
{
  m_bg_style = bgst;

  request_redraw();
}




void
core::
push_underlay(const gbstd::image&  img, gbstd::point  pt) noexcept
{
  m_underlay_stack.emplace_back(img,pt.x,pt.y,m_canvas.get_width(),m_canvas.get_height());
  m_underlay_points.emplace_back(pt);
}


void
core::
pop_underlay() noexcept
{
  m_underlay_stack.pop_back();
  m_underlay_points.pop_back();
}


void
core::
rebase_underlay_stack(const gbstd::image&  img) noexcept
{
  m_underlay_stack.clear();

  int  img_w = img.get_width() ;
  int  img_h = img.get_height();

  int  w = m_canvas.get_width() ;
  int  h = m_canvas.get_height();

    for(auto&  pt: m_underlay_points)
    {
        if(((pt.x+w) < img_w) &&
           ((pt.y+h) < img_h))
        {
          m_underlay_stack.emplace_back(img,w*pt.x,h*pt.y,w,h);
        }
    }
}


void
core::
show_underlay() noexcept
{
  m_whether_show_underlay = true;

  request_redraw();
}


void
core::
hide_underlay() noexcept
{
  m_whether_show_underlay = false;

  request_redraw();
}


void
core::
render_background(int  pixel_size, const gbstd::canvas&  cv) const noexcept
{
  int  w = cv.get_width() ;
  int  h = cv.get_height();

  cv.draw_stripe_rectangle(m_bg_style.first_color,m_bg_style.second_color,pixel_size/2,0,0,w,h);
}


void
core::
render_underlay(int  pixel_size, const gbstd::canvas&  cv) const noexcept
{
  int  w = cv.get_width() ;
  int  h = cv.get_height();

    for(auto&  ulcv: m_underlay_stack)
    {
        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto&  pix = *ulcv.get_pixel_pointer(x,y);

            if(pix.color)
            {
              cv.fill_rectangle(pix.color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
            }
        }}
    }
}


void
core::
render(const gbstd::canvas&  cv) noexcept
{
  int  w = m_canvas.get_width() ;
  int  h = m_canvas.get_height();

  render_background(m_pixel_size,cv);

    if(m_whether_show_underlay)
    {
      render_underlay(  m_pixel_size,cv);
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




}




