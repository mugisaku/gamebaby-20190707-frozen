#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


void
context::
build_core() noexcept
{
  m_cursor_label = new label(u"X: -- Y: -- PIX: ---",colors::black);

  m_core = new core([](core_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_image_modified())
      {
        ctx.m_menu->request_redraw();
      }


    string_form  sf;

    auto&  pt = evt->get_current_point();

    auto  color = evt->get_pixel(pt.x,pt.y).color;

      if(color)
      {
        int  r = color.get_r7();
        int  g = color.get_g7();
        int  b = color.get_b7();

        sf("X: %2d Y: %2d PIX: %d%d%d",pt.x,pt.y,r,g,b);
      }

    else
      {
        sf("X: %2d Y: %2d PIX: ---",pt.x,pt.y);
      }


    ctx.m_cursor_label->modify_text(sf());
  });


  m_core->set_pixel_size(8);
  m_core->set_userdata(this);


  m_core_frame = new frame("canvas",make_column({m_core,m_cursor_label}));

  m_core_frame->set_line_color(colors::white);
}




}




