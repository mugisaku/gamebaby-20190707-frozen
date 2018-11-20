#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




void
canvas::
draw_dot(color  i, int  x, int  y) const noexcept
{
    if(i)
    {
      get_pixel_pointer(x,y)->color = i;
    }
}


void
canvas::
draw_dot_safely(color  i, int  x, int  y) const noexcept
{
    if((x >=        0) &&
       (x <   m_width) &&
       (y >=        0) &&
       (y <  m_height))
    {
      draw_dot(i,x,y);
    }
}


void
canvas::
draw_vline(color  i, int  x, int  y, int  l) const noexcept
{
    while(l--)
    {
      draw_dot(i,x,y++);
    }
}


void
canvas::
draw_vline_safely(color  i, int  x, int  y, int  l) const noexcept
{
    if((x >=       0) &&
       (x <  m_width))
    {
        if(y < 0)
        {
          l += y    ;
               y = 0;
        }

      else
        if(y+l >= m_height)
        {
          l = m_height-y;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(i,x,y);

              y += 1;
            }
        }
    }
}


void
canvas::
draw_hline(color  i, int  x, int  y, int  l) const noexcept
{
    while(l--)
    {
      draw_dot(i,x++,y);
    }
}


void
canvas::
draw_hline_safely(color  i, int  x, int  y, int  l) const noexcept
{
    if((y >=        0) &&
       (y <  m_height))
    {
        if(x < 0)
        {
          l += x    ;
               x = 0;
        }

      else
        if(x+l >= m_width)
        {
          l = m_width-x;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(i,x,y);

              x += 1;
            }
        }
    }
}


void
canvas::
draw_line(color  i, int  x0, int  y0, int  x1, int  y1) const noexcept
{
  liner  lnr(x0,y0,x1,y1);

    for(;;)
    {
      draw_dot(i,lnr.get_x(),lnr.get_y());

        if(!lnr.get_distance())
        {
          break;
        }


      lnr.step();
    }
}


void
canvas::
draw_line_safely(color  i, int  x0, int  y0, int  x1, int  y1) const noexcept
{
  liner  lnr(x0,y0,x1,y1);

    for(;;)
    {
      draw_dot_safely(i,lnr.get_x(),lnr.get_y());

        if(!lnr.get_distance())
        {
          break;
        }


      lnr.step();
    }
}


void
canvas::
draw_rectangle(color  i, int  x, int  y, int  w, int  h) const noexcept
{
    if(i)
    {
      draw_hline(i,x,y    ,w);
      draw_hline(i,x,y+h-1,w);

      draw_vline(i,x    ,y+1,h-2);
      draw_vline(i,x+w-1,y+1,h-2);
    }
}


void
canvas::
draw_rectangle_safely(color  i, int  x, int  y, int  w, int  h) const noexcept
{
  draw_hline_safely(i,x,y    ,w);
  draw_hline_safely(i,x,y+h-1,w);

  draw_vline_safely(i,x    ,y+1,h-2);
  draw_vline_safely(i,x+w-1,y+1,h-2);
}


void
canvas::
fill_rectangle(color  i, int  x, int  y, int  w, int  h) const noexcept
{
    while(h--)
    {
      draw_hline(i,x,y,w);

      ++y;
    }
}


void
canvas::
fill_rectangle_safely(color  i, int  x, int  y, int  w, int  h) const noexcept
{
    while(h--)
    {
      draw_hline_safely(i,x,y,w);

      ++y;
    }
}




void
canvas::
draw_double_rectangle(color  out, color  in, int  x, int  y, int  w, int  h) const noexcept
{
  draw_rectangle(out,x  ,y  ,w  ,h  );
  draw_rectangle( in,x+1,y+1,w-2,h-2);
}


void
canvas::
draw_stripe_rectangle(color  first, color  second, int  interval, int  x, int  y, int  w, int  h) const noexcept
{
    if(interval)
    {
        for(;;)
        {
            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  return;
                }


              draw_hline(first,x,y++,w);
            }


            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  return;
                }


              draw_hline(second,x,y++,w);
            }
        }
    }
}




void
canvas::
draw_glyph(const character_color&  chr_color, const glyph&  gl, int  x, int  y) const noexcept
{
  auto  p = &gl.data[0];

    for(int  yy = 0;  yy < g_font_height;  yy += 1)
    {
      auto   dst = get_pixel_pointer(x,y+yy);
      auto  code = *p++;

        for(int  xx = 0;  xx < g_font_width;  xx += 1)
        {
          auto  color = chr_color[code>>14];

            if(color)
            {
              dst->color = color;
            }


           dst  += 1;
          code <<= 2;
        }
    }
}


void
canvas::
draw_glyph_safely(const character_color&  chr_color, const glyph&  gl, int  x, int  y) const noexcept
{
  auto  p = &gl.data[0];

    for(int  yy = 0;  yy < g_font_height;  yy += 1)
    {
      auto   dst = get_pixel_pointer(x,y+yy);
      auto  code = *p++;

      int  yyy = y+yy;

        if((yyy >= 0) && (yyy < get_height()))
        {
            for(int  xx = 0;  xx < g_font_width;  xx += 1)
            {
              int  xxx = x+xx;

                if((xxx >= 0) && (xxx < get_width()))
                {
                  auto  color = chr_color[code>>14];

                    if(color)
                    {
                      dst->color = color;
                    }
                }


               dst  += 1;
              code <<= 2;
            }
        }
    }
}


void
canvas::
draw_character(const character&  c, int  x, int  y) const noexcept
{
  auto  gl = get_glyph(c.unicode);

    if(gl)
    {
      draw_glyph(c.color,*gl,x,y);
    }
}




void
canvas::
draw_string(const character_color&  color, const char*  s, int  x, int  y) const noexcept
{
  utf8_decoder  dec(s);

  character  c;

  c.color = color;

    while(dec)
    {
      c.unicode = static_cast<char16_t>(dec());

        if(!c.unicode)
        {
          break;
        }


      draw_character(c,x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string(const character_color&  color, const char16_t*  s, int  x, int  y) const noexcept
{
  character  c;

  c.color = color;

    for(;;)
    {
      c.unicode = *s++;

        if(!c.unicode)
        {
          break;
        }


      draw_character(c,x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_character_safely(const character&  c, int  x, int  y) const noexcept
{
  auto  gl = get_glyph(c.unicode);

    if(gl)
    {
      draw_glyph_safely(c.color,*gl,x,y);
    }
}




void
canvas::
draw_string_safely(const character_color&  color, const char*  s, int  x, int  y) const noexcept
{
  utf8_decoder  dec(s);

  character  c;

  c.color = color;

    while(dec)
    {
      c.unicode = static_cast<char16_t>(dec());

        if(!c.unicode)
        {
          break;
        }


      draw_character(c,x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string_safely(const character_color&  color, const char16_t*  s, int  x, int  y) const noexcept
{
  character  c;

  c.color = color;

    for(;;)
    {
      c.unicode = *s++;

        if(!c.unicode)
        {
          break;
        }


      draw_character(c,x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_sprite(const sprite&  spr, int  x, int  y) const noexcept
{
  int  spr_x = spr.rectangle.x;
  int  spr_y = spr.rectangle.y;
  int  spr_w = spr.rectangle.w;
  int  spr_h = spr.rectangle.h;

    if(spr_x < 0)
    {
      spr_w += spr_x    ;
               spr_x = 0;
    }

  else
    if(spr_x >= m_width)
    {
      return;
    }


    if((spr_x+spr_w) >= m_width)
    {
      spr_w = m_width-spr_x;
    }


    if(spr_w <= 0)
    {
      return;
    }


    if(spr_y < 0)
    {
      spr_h += spr_y    ;
               spr_y = 0;
    }

  else
    if(spr_y >= m_height)
    {
      return;
    }


    if((spr_y+spr_h) >= m_height)
    {
      spr_h = m_height-spr_y;
    }


    if(spr_h <= 0)
    {
      return;
    }


    for(int  yy = 0;  yy < spr_h;  ++yy){
    for(int  xx = 0;  xx < spr_w;  ++xx){
      auto&  pix = *spr.image->get_pixel_pointer(spr_x+xx,spr_y+yy);

      draw_dot(pix.color,x+xx,y+yy);
    }}
}




namespace{
bool
correct(canvas&  src_cv, int&  dst_x, int&  dst_y, int  dst_w, int  dst_h) noexcept
{
  auto  src_w = src_cv.get_width() ;
  auto  src_h = src_cv.get_height();

    if(dst_x < 0)
    {
        if((src_w+dst_x) <= 0)
        {
          return false;
        }


      src_cv.move_x(-dst_x);

      src_w += dst_x    ;
               dst_x = 0;
    }


    if(dst_y < 0)
    {
        if((src_h+dst_y) <= 0)
        {
          return false;
        }


      src_cv.move_y(-dst_y);

      src_h += dst_y    ;
               dst_y = 0;
    }


    if((dst_x+src_w) >= dst_w)
    {
      src_w = (dst_w-dst_x);

        if(src_w <= 0)
        {
          return false;
        }
    }


    if((dst_y+src_h) >= dst_h)
    {
      src_h = (dst_h-dst_y);

        if(src_h <= 0)
        {
          return false;
        }
    }


  src_cv.set_width( src_w);
  src_cv.set_height(src_h);

  return true;
}


template<bool  COPY>
void
transfer(const canvas&  orig_src, const canvas&  dst, int  dst_x, int  dst_y) noexcept
{
  canvas  src = orig_src;

    if(correct(src,dst_x,dst_y,dst.get_width(),dst.get_height()))
    {
      auto  src_pixptr_base = src.get_pixel_pointer(    0,    0);
      auto  dst_pixptr_base = dst.get_pixel_pointer(dst_x,dst_y);

      auto  src_w = src.get_width() ;
      auto  src_h = src.get_height();

        for(int  y = 0;  y < src_h;  ++y)
        {
          auto  dst_pixptr = dst_pixptr_base                         ;
                             dst_pixptr_base += dst.get_image_width();

          auto  src_pixptr = src_pixptr_base                         ;
                             src_pixptr_base += src.get_image_width();

            for(int  x = 0;  x < src_w;  ++x)
            {
              auto&  pix = *src_pixptr;

                if(COPY || pix.color)
                {
                  *dst_pixptr = pix;
                }


              ++dst_pixptr;
              ++src_pixptr;
            }
        }
    }
}
}


void
canvas::
draw_canvas(const canvas&  cv, int   x, int  y) const noexcept
{
  transfer<false>(cv,*this,x,y);
}


void
canvas::
copy_canvas(const canvas&  cv, int   x, int  y) const noexcept
{
  transfer<true>(cv,*this,x,y);
}


void
canvas::
draw_canvas(const canvas&  cv, int  permill, int  x, int  y) const noexcept
{
  int  src_w = cv.get_width() ;
  int  src_h = cv.get_height();

  int  w = ((src_w<<10)/100*permill)>>10;
  int  h = ((src_h<<10)/100*permill)>>10;

  int  x_unit = (src_w<<10)/w;
  int  y_unit = (src_h<<10)/h;

    for(int  yy = 0;  yy < h;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      int  xxx = (x_unit*xx)>>10;
      int  yyy = (y_unit*yy)>>10;

        if((xxx < src_w) &&
           (yyy < src_h))
        {
          auto  pix = *cv.get_pixel_pointer(xxx,yyy);

          draw_dot(pix,x+xx,y+yy);
        }
    }}
}




void
canvas::
blend_canvas(const canvas&  cv, int  x, int  y) const noexcept
{
  canvas  src_cv = cv;

    if(correct(src_cv,x,y,get_width(),get_height()))
    {
      auto  src_pixptr_base = cv.get_pixel_pointer(0,0);
      auto  dst_pixptr_base =    get_pixel_pointer(x,y);

      auto  src_w = src_cv.get_width() ;
      auto  src_h = src_cv.get_height();

        for(int  yy = 0;  yy < src_h;  ++yy)
        {
          auto  dst_pixptr = dst_pixptr_base                     ;
                             dst_pixptr_base += get_image_width();

          auto  src_pixptr = src_pixptr_base                            ;
                             src_pixptr_base += src_cv.get_image_width();

            for(int  xx = 0;  xx < src_w;  ++xx)
            {
              auto&  pix = *src_pixptr;

                if(pix.color && (dst_pixptr->z <= pix.z))
                {
                  *dst_pixptr = pix;
                }


              ++dst_pixptr;
              ++src_pixptr;
            }
        }
    }
}


void
canvas::
blend_canvas(const canvas&  cv, int  x, int  y, int  z_base, int  z_add_amount) const noexcept
{
  canvas  src_cv = cv;

    if(correct(src_cv,x,y,get_width(),get_height()))
    {
      auto  src_pixptr_base = cv.get_pixel_pointer(0,0);
      auto  dst_pixptr_base =    get_pixel_pointer(x,y);

      auto  src_w = src_cv.get_width() ;
      auto  src_h = src_cv.get_height();

        for(int  yy = 0;  yy < src_h;  ++yy)
        {
          auto  dst_pixptr = dst_pixptr_base                     ;
                             dst_pixptr_base += get_image_width();

          auto  src_pixptr = src_pixptr_base                            ;
                             src_pixptr_base += src_cv.get_image_width();

            for(int  xx = 0;  xx < src_w;  ++xx)
            {
              auto&  pix = *src_pixptr;

                if(pix.color && (dst_pixptr->z <= z_base))
                {
                  pix.z = z_base;

                  *dst_pixptr = pix;
                }


              ++dst_pixptr;
              ++src_pixptr;
            }


          z_base += z_add_amount;
        }
    }
}




}





