#include"sdl.hpp"
#include"libgbstd/image.hpp"
#include"libgbpng/png.hpp"
#include<vector>




using namespace gbpng;
using namespace gbstd;


namespace{


constexpr int  g_screen_w = 400;
constexpr int  g_screen_h = 400;

constexpr uint8_t  fg_l = 0x00;
constexpr uint8_t  bg_l = 0xFF;


constexpr int  g_static_line_width = 1;
constexpr int  g_moving_line_width = 1;


template<int  N>
void
draw_vline(direct_color_image&  img, int  x, int  y, int  n, uint8_t  l) noexcept
{
  x -= N/2;

    for(int  nn = 0;  nn < N;  ++nn)
    {
        if((x >= 0) && (x < g_screen_w))
        {
          auto  p = img.get_pixel_pointer(x,y);

            for(int  yy = 0;  yy < n;  ++yy)
            {
              p[0] = l;
              p[1] = l;
              p[2] = l;
              p[3] = 0xFF;

              p += 4*g_screen_w;
            }
        }


      ++x;
    }
}


template<int  N>
void
draw_hline(direct_color_image&  img, int  x, int  y, int  n, uint8_t  l) noexcept
{
  y -= N/2;

    for(int  nn = 0;  nn < N;  ++nn)
    {
        if((y >= 0) && (y < g_screen_h))
        {
          auto  p = img.get_pixel_pointer(x,y);

            for(int  xx = 0;  xx < n;  ++xx)
            {
              *p++ = l;
              *p++ = l;
              *p++ = l;
              *p++ = 0xFF;
            }
        }


      ++y;
    }
}




struct
rectangle
{
  int  x;
  int  y;
  int  w;
  int  h;

};


template<int  N>
void
draw_rect(direct_color_image&  img, rectangle  rect, uint8_t  l) noexcept
{
    if((rect.w >= 0) && (rect.h >= 0))
    {
      draw_hline<N>(img,rect.x         ,rect.y         ,rect.w,l);
      draw_hline<N>(img,rect.x         ,rect.y+rect.h-1,rect.w,l);
      draw_vline<N>(img,rect.x         ,rect.y         ,rect.h,l);
      draw_vline<N>(img,rect.x+rect.w-1,rect.y         ,rect.h,l);
    }
}




constexpr int
rmul(int  value, int  denominator, int  numerator) noexcept
{
  constexpr int  shift_amount = 16;

  return (((value<<shift_amount)/denominator)*numerator)>>shift_amount;
}


constexpr
rectangle
get_rect(int  denominator, int  numerator) noexcept
{
  return {(g_screen_w/2)-rmul(g_screen_w/2,denominator,numerator),
          (g_screen_h/2)-rmul(g_screen_h/2,denominator,numerator),
                         rmul(g_screen_w  ,denominator,numerator),
                         rmul(g_screen_h  ,denominator,numerator)};
}




void
put(uint8_t  l, uint8_t*  p) noexcept
{
  *p++ =   l;
  *p++ =   l;
  *p++ =   l;
  *p++ = 255;
}


void
draw_line(direct_color_image&  img) noexcept
{
  int  center_x = g_screen_w/2;
  int  center_y = g_screen_h/2;

  gbstd::liner  ln(0,0,center_x,center_y);

    for(;;)
    {
      int  x = ln.get_x();
      int  y = ln.get_y();

      put(fg_l,img.get_pixel_pointer(             x,             y));
      put(fg_l,img.get_pixel_pointer(g_screen_w-1-x,             y));
      put(fg_l,img.get_pixel_pointer(             x,g_screen_h-1-y));
      put(fg_l,img.get_pixel_pointer(g_screen_w-1-x,g_screen_h-1-y));

        if(!ln.get_distance())
        {
          break;
        }


      ln.step();
    }
}


direct_color_image
make_base_image(const direct_color_image&  src) noexcept
{
  auto  img = src;

  auto  p = img.get_pixel_pointer(0,0);

    for(int  y = 0;  y < g_screen_h;  ++y){
    for(int  x = 0;  x < g_screen_w;  ++x){
      *p++ = bg_l;
      *p++ = bg_l;
      *p++ = bg_l;
      *p++ = 0xFF;
    }}


  draw_line(img);

  p = img.get_pixel_pointer(g_screen_w/2,0);

    for(int  y = 0;  y < g_screen_h;  ++y)
    {
      p[0] = fg_l;
      p[1] = fg_l;
      p[2] = fg_l;
      p[3] = 0xFF;

      p += 4*g_screen_w;
    }


  p = img.get_pixel_pointer(0,g_screen_h/2);

    for(int  x = 0;  x < g_screen_w;  ++x)
    {
      *p++ = fg_l;
      *p++ = fg_l;
      *p++ = fg_l;
      *p++ = 0xFF;
    }


    for(int  n = 0;  n < 6;  ++n)
    {
      auto  rect = get_rect(g_screen_w,16<<n);

      draw_rect<g_static_line_width>(img,rect,fg_l);
    }


  return std::move(img);
}


void
save(const direct_color_image&  img) noexcept
{
  static int  i = 0;

  char  buf[256];

  snprintf(buf,sizeof(buf),"%03d.png",i++);

  img.write_png_to_file(buf,pixel_format::rgba,8);
}


}




int
main(int  argc, char**  argv)
{
  constexpr int  delay_ms = 60;

  image_header  ihdr(g_screen_w,g_screen_h);

  ihdr.set_bit_depth(1);
  ihdr.set_pixel_format(pixel_format::grayscale);


  animation_builder  ani(ihdr,delay_ms);

  direct_color_image  img;

  img.allocate(g_screen_w,g_screen_h);


  auto  base_img = make_base_image(img);

  constexpr int  den = 200;
  constexpr int  shift_amount = 16;

    for(int  n = den;  n > 5;  --n)
    {
printf("\r%3d processing...",n);
fflush(stdout);
      img = base_img;

      auto  rect = get_rect(den,n);

      draw_rect<2>(img,rect,fg_l);

      ani.append(img);

      save(img);
    }


    for(int  n = 5; n < den;  ++n)
    {
printf("\r%3d processing...",n);
fflush(stdout);
      img = base_img;

      auto  rect = get_rect(den,0);

      draw_rect<2>(img,rect,fg_l);

      ani.append(img);

      save(img);
    }


printf("finished\n");
fflush(stdout);

  auto  ls = ani.build();

  ls.write_png_to_file("eyesquat.png");

  char  buf[256];

  snprintf(buf,sizeof(buf),"../mkgif.sh %d",delay_ms);

  system(buf);


  return 0;
}




