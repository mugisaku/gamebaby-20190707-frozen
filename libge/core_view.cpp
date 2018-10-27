#include"libge/ge.hpp"




namespace ge{


core_view::
core_view(core_display&  dsp) noexcept:
m_display(&dsp)
{
}




void
core_view::
process_before_reform() noexcept
{
  int  w = m_canvas.get_width();
  int  h = m_canvas.get_height();

  set_content_width( m_display->get_pixel_size()*w);
  set_content_height(m_display->get_pixel_size()*h);
}


void
core_view::
render(const gbstd::canvas&  cv) noexcept
{
  int  w = m_canvas.get_width() ;
  int  h = m_canvas.get_height();

  m_display->render_background(cv);

  m_display->render_canvas(m_canvas,cv);
}




}




