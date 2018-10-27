#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


void
context::
build_core() noexcept
{
  character_color  color(colors::black,colors::white,colors::black,colors::black);

  m_cursor_label = new label(u"X: -- Y: -- PIX: ---",color);

  m_core = new core(m_paint,m_display,[](core_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_painting_cursor_moved())
      {
        string_form  sf;

        auto  pt = evt->get_paint().get_drawing_point();

        auto  color = evt->get_paint().get_current_pixel().color;

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
      }
  });


  m_display.set_pixel_size(8);
  m_core->set_userdata(this);


  m_core_frame = new frame("canvas",make_column({m_cursor_label,m_core}));

  m_core_frame->set_line_color(colors::white);
}




}




