#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




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
draw_string(const character_color&  color, std::string_view  sv, int  x, int  y) const noexcept
{
  utf8_decoder  dec(sv.data());

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
draw_string(const character_color&  color, std::u16string_view  sv, int  x, int  y) const noexcept
{
  character  c;

  c.color = color;

    for(auto  u16: sv)
    {
      c.unicode = u16;

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
draw_string_safely(const character_color&  color, std::string_view  sv, int  x, int  y) const noexcept
{
  utf8_decoder  dec(sv.data());

  character  c;

  c.color = color;

    while(dec)
    {
      c.unicode = static_cast<char16_t>(dec());

        if(!c.unicode)
        {
          break;
        }


      draw_character_safely(c,x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string_safely(const character_color&  color, std::u16string_view  sv, int  x, int  y) const noexcept
{
  character  c;

  c.color = color;

    for(auto  u16: sv)
    {
      c.unicode = u16;

      draw_character_safely(c,x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string_as_right_align(const character_color&  color, std::u16string_view  sv, int  x, int  y) const noexcept
{
  character  c;

  c.color = color;

  auto  it     = sv.rbegin();
  auto  it_end = sv.rend();

  x -= g_font_width;

    while(it != it_end)
    {
      c.unicode = *it++;

      draw_character_safely(c,x,y);

      x -= g_font_width;
    }
}


void
canvas::
draw_string_safely_as_right_align(const character_color&  color, std::u16string_view  sv, int  x, int  y) const noexcept
{
  character  c;

  c.color = color;

  auto  it     = sv.rbegin();
  auto  it_end = sv.rend();

  x -= g_font_width;

    while(it != it_end)
    {
      c.unicode = *it++;

      draw_character_safely(c,x,y);

      x -= g_font_width;
    }
}




void
canvas::
draw_typewriter(const typewriter&  tw, int  x, int  y) const noexcept
{
  int  w = tw.get_width() ;
  int  h = tw.get_height();

    for(int  yy = 0;  yy < h;  ++yy)
    {
      auto  cp = tw.get_character_pointer(0,yy);

      int  dst_x = x;

        for(int  xx = 0;  xx < w;  ++xx)
        {
          draw_character(*cp++,dst_x,y);

          dst_x += g_font_width;
        }


      y += g_font_height;
    }
}




}





