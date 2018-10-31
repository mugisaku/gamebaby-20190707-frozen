#include"libge/ge_context.hpp"




namespace ge{


using namespace gbstd;


constexpr int  g_cell_size = 12;

constexpr int     top = 1;
constexpr int  bottom = 2;
constexpr int    left = 4;
constexpr int   right = 8;




previewer::
previewer(const image&  img) noexcept:
m_a_base(     img,g_cell_size*0,0,g_cell_size,g_cell_size),
m_b_base(     img,g_cell_size*1,0,g_cell_size,g_cell_size),
m_top_edge(   img,g_cell_size*2,0,g_cell_size,g_cell_size),
m_bottom_edge(img,g_cell_size*3,0,g_cell_size,g_cell_size),
m_left_edge(  img,g_cell_size*4,0,g_cell_size,g_cell_size),
m_right_edge( img,g_cell_size*5,0,g_cell_size,g_cell_size)
{
}




void
previewer::
process_before_reform() noexcept
{
  auto&  ctx = *get_userdata<context>();

  set_content_width( g_cell_size*2*5);
  set_content_height(g_cell_size*2*4);
}


void
previewer::
render_product(point  pt, int  flags, const gbstd::canvas&  cv) noexcept
{
  int  x = g_cell_size*2*pt.x;
  int  y = g_cell_size*2*pt.y;

  cv.draw_canvas(m_a_base,x            ,y            );
  cv.draw_canvas(m_b_base,x+g_cell_size,y            );
  cv.draw_canvas(m_b_base,x            ,y+g_cell_size);
  cv.draw_canvas(m_a_base,x+g_cell_size,y+g_cell_size);

    if(flags&left)
    {
      cv.draw_canvas(m_left_edge,x,y            );
      cv.draw_canvas(m_left_edge,x,y+g_cell_size);
    }


    if(flags&right)
    {
      cv.draw_canvas(m_right_edge,x+g_cell_size,y            );
      cv.draw_canvas(m_right_edge,x+g_cell_size,y+g_cell_size);
    }


    if(flags&top)
    {
      cv.draw_canvas(m_top_edge,x            ,y);
      cv.draw_canvas(m_top_edge,x+g_cell_size,y);
    }


    if(flags&bottom)
    {
      cv.draw_canvas(m_bottom_edge,x            ,y+g_cell_size);
      cv.draw_canvas(m_bottom_edge,x+g_cell_size,y+g_cell_size);
    }
}


void
previewer::
render(const canvas&  cv) noexcept
{
  auto&  ctx = *get_userdata<context2>();

  cv.fill(color());

  ctx.m_core->get_display().render_background(cv,2);

  render_product({0,0},top|left,cv);
  render_product({1,0},top,cv);
  render_product({2,0},top|right,cv);
  render_product({0,1},left,cv);
  render_product({1,1},0,cv);
  render_product({2,1},right,cv);
  render_product({0,2},bottom|left,cv);
  render_product({1,2},bottom,cv);
  render_product({2,2},bottom|right,cv);

  render_product({3,0},bottom|top,cv);
  render_product({4,0},left|right,cv);

  render_product({3,1},bottom|top|left,cv);
  render_product({4,1},bottom|top|right,cv);

  render_product({3,2},bottom|left|right,cv);
  render_product({4,2},top|left|right,cv);

  render_product({0,3},top|bottom|left|right,cv);
}




}




