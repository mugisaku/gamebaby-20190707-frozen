#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


void
color_handler::
holder_callback(color_holder&  holder, gbstd::color  color) noexcept
{
  auto&  hndl = *holder.get_userdata<color_handler>();
    
  hndl.m_maker->set_color(color);
}


void
color_handler::
maker_callback(color_maker&  maker, gbstd::color  color) noexcept
{
  auto&  hndl = *maker.get_userdata<color_handler>();

  hndl.m_holder->set_color(color);

  hndl.m_paint->set_drawing_color(color);
}




color_handler::
color_handler(core_paint&  pai) noexcept
{
  auto  color_list = {
    colors::black,
    colors::dark_gray,
    colors::gray,
    colors::light_gray,
    colors::white,
    colors::red,
    colors::green,
    colors::blue,
    colors::yellow,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
  };


  m_holder = new color_holder(color_list,holder_callback);
  m_maker  = new color_maker(maker_callback);
  m_paint  = &pai;


  m_maker->set_userdata(this);
  m_holder->set_userdata(this);
}




}




