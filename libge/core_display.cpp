#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


void
core_display::
first_callback(gbstd::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  dsp = *evt->get_userdata<core_display>();

      dsp.m_bg_style.first_color = *dsp.m_color;

        for(auto  ptr: dsp.m_widget_list)
        {
          ptr->request_redraw();
        }
    }
}


void
core_display::
second_callback(gbstd::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  dsp = *evt->get_userdata<core_display>();

      dsp.m_bg_style.second_color = *dsp.m_color;

        for(auto  ptr: dsp.m_widget_list)
        {
          ptr->request_redraw();
        }
    }
}




core_display::
core_display() noexcept
{
  m_bg_style.first_color  = gbstd::color(0,0,7);
  m_bg_style.second_color = gbstd::color(2,2,5);

  auto  ch1bg_btn = new button(new label(u"Change bg1 color",colors::black), first_callback);
  auto  ch2bg_btn = new button(new label(u"Change bg2 color",colors::black),second_callback);

  ch1bg_btn->set_userdata(this);
  ch2bg_btn->set_userdata(this);

  m_container = make_column({ch1bg_btn,ch2bg_btn});
}




void
core_display::
reset(const gbstd::color&  color, std::initializer_list<gbstd::widget*>  ls) noexcept
{
  m_color = &color;

  m_widget_list = ls;
}




void
core_display::
push_underlay(gbstd::image&  img, gbstd::point  pt, int  w, int  h) noexcept
{
  m_underlay_points.emplace_back(pt);

  m_underlay_stack.emplace_back(img,w*pt.x,h*pt.y,w,h);
}


void
core_display::
pop_underlay() noexcept
{
  m_underlay_stack.pop_back();
  m_underlay_points.pop_back();
}


void
core_display::
clear_underlay_stack() noexcept
{
  m_underlay_stack.clear();
  m_underlay_points.clear();
}


void
core_display::
rebase_underlay_stack(gbstd::image&  img, int  w, int  h) noexcept
{
  int  img_w = img.get_width() ;
  int  img_h = img.get_height();

  m_underlay_stack.clear();

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
core_display::
render_rect(gbstd::rectangle  rect, const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  cv.draw_double_rectangle(gbstd::colors::white,gbstd::colors::black,
    pixel_size*rect.x,
    pixel_size*rect.y,
    pixel_size*rect.w,
    pixel_size*rect.h
  );
}


void
core_display::
render_grid(const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  int  w = cv.get_width() ;
  int  h = cv.get_height();

  int  ww = w/pixel_size;
  int  hh = h/pixel_size;

  int  x_sz = ww/4;
  int  y_sz = hh/4;

    for(int  y = 0;  y < hh;  ++y)
    {
      auto  color = (y%y_sz)? gbstd::colors::gray
                            : gbstd::colors::light_gray;

      cv.draw_hline(color,0,pixel_size*y,w);
    }


    for(int  x = 0;  x < ww;  ++x)
    {
      auto  color = (x%x_sz)? gbstd::colors::gray
                            : gbstd::colors::light_gray;

      cv.draw_vline(color,pixel_size*x,0,h);
    }


  cv.draw_hline(gbstd::colors::white,0,(h/2)  ,w);
  cv.draw_vline(gbstd::colors::white,  (w/2),0,h);
}


void
core_display::
render_background(const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  int  w = cv.get_width() ;
  int  h = cv.get_height();

  cv.draw_stripe_rectangle(m_bg_style.first_color,m_bg_style.second_color,pixel_size/2,0,0,w,h);
}


void
core_display::
render_underlay(const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


    for(auto&  underlay_cv: m_underlay_stack)
    {
      int  w = underlay_cv.get_width() ;
      int  h = underlay_cv.get_height();

        for(int  y = 0;  y < h;  ++y){
        for(int  x = 0;  x < w;  ++x){
          auto&  pix = *underlay_cv.get_pixel_pointer(x,y);

            if(pix.color)
            {
              cv.fill_rectangle(pix.color,pixel_size*x,pixel_size*y,pixel_size,pixel_size);
            }
        }}
    }
}


void
core_display::
render_canvas(const gbstd::canvas&  src_cv, const gbstd::canvas&  dst_cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  int  w = src_cv.get_width() ;
  int  h = src_cv.get_height();

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  pix = *src_cv.get_pixel_pointer(x,y);

        if(pix.color)
        {
          dst_cv.fill_rectangle(pix.color,pixel_size*x,pixel_size*y,pixel_size,pixel_size);
        }
    }}
}




}




