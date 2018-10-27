#include"libge/ge.hpp"




namespace ge{


core::
core(core_paint&  pai, core_display&  dsp, void  (*callback)(core_event  evt)) noexcept:
m_paint(&pai),
m_display(&dsp),
m_callback(callback)
{
}




void
core::
process_before_reform() noexcept
{
  int  w = m_canvas.get_width();
  int  h = m_canvas.get_height();

  set_content_width( m_display->get_pixel_size()*w);
  set_content_height(m_display->get_pixel_size()*h);
}


void
core::
do_on_mouse_enter() noexcept
{
  m_paint->reset(m_canvas);

  request_redraw();
}


void
core::
do_on_mouse_leave() noexcept
{
  m_paint->cancel_drawing();

  request_redraw();
}




void
core::
do_on_mouse_act(gbstd::point  mouse_pos) noexcept
{
  int  x = mouse_pos.x/m_display->get_pixel_size();
  int  y = mouse_pos.y/m_display->get_pixel_size();

  gbstd::point  pt(x,y);

    if(pt != m_paint->get_drawing_point())
    {
      m_paint->set_drawing_point(pt);

        if(m_callback)
        {
          m_callback({*this,core_event::kind::painting_cursor_moved});
        }
    }


  (*m_paint)();
}


void
core::
render(const gbstd::canvas&  cv) noexcept
{
  int  w = m_canvas.get_width() ;
  int  h = m_canvas.get_height();

  m_display->render_background(cv);

    if(m_display->test_whether_show_underlay())
    {
      m_display->render_underlay(cv);
    }


  m_display->render_canvas(m_canvas,cv);

    if(m_display->test_whether_show_grid())
    {
       m_display->render_grid(cv);
    }


  auto  rect = m_paint->get_operation_rectangle();

    if(rect.w && rect.h)
    {
      m_display->render_rect(rect,cv);
    }
}




}




