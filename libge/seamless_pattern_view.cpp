#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


constexpr int  g_number_of_tiles = 3;


seamless_pattern_view::
seamless_pattern_view(context&  ctx) noexcept:
widget(ctx.get_cell_width()*g_number_of_tiles,ctx.get_cell_height()*g_number_of_tiles)
{
  gbstd::set_userdata({this},&ctx);
}




void
seamless_pattern_view::
process_before_reform() noexcept
{
  auto&  ctx = *get_userdata<context>();

  int  w = ctx.get_cell_width() ;
  int  h = ctx.get_cell_height();

  set_content_width( w*g_number_of_tiles);
  set_content_height(h*g_number_of_tiles);
}


void
seamless_pattern_view::
render(const canvas&  cv) noexcept
{
  auto&  ctx = *get_userdata<context>();

  ctx.m_core->get_display().render_background(cv,2);


  auto  cocv = ctx.m_core->get_canvas();

  int  w = cocv.get_width() ;
  int  h = cocv.get_height();

    for(int  y = 0;  y < g_number_of_tiles;  ++y){
    for(int  x = 0;  x < g_number_of_tiles;  ++x){
      cv.draw_canvas(cocv,w*x,h*y);
    }}
}


}




