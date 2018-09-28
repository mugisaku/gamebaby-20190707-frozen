#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




image::
image(int  w, int  h, std::initializer_list<int>  ls) noexcept
{
  resize(w,h);

  int  n = std::min(w*h,(int)ls.size());

  auto  src = ls.begin();
  auto  dst =    begin();

    while(n--)
    {
      *dst++ = pixel(color(*src++));
    }
}


image::
image(int  w, int  h, std::initializer_list<color>  ls) noexcept
{
  resize(w,h);

  int  n = std::min(w*h,(int)ls.size());

  auto  src = ls.begin();
  auto  dst =    begin();

    while(n--)
    {
      *dst++ = *src++;
    }
}





image&
image::
assign(const image&  rhs) noexcept
{
    if(this != &rhs)
    {
      resize(rhs.m_width,rhs.m_height);
    }


  return *this;
}


image&
image::
assign(image&&  rhs) noexcept
{
    if(this != &rhs)
    {
      resize(0,0);

      std::swap(m_pixels,rhs.m_pixels);
      std::swap(m_width ,rhs.m_width );
      std::swap(m_height,rhs.m_height);
    }


  return *this;
}




void
image::
resize(int  w, int  h) noexcept
{
    if((w == m_width ) &&
       (h == m_height))
    {
      return;
    }


    if(!w || !h)
    {
      static pixel  null;

      delete[] m_pixels        ;
               m_pixels = &null;

      m_width  = 0;
      m_height = 0;

      return;
    }


  int  number_of_pixels = w*h;

    if(number_of_pixels > (m_width*m_height))
    {
      delete[] m_pixels                              ;
               m_pixels = new pixel[number_of_pixels];
    }

  m_width  = w;
  m_height = h;
}




void
image::
fill(color  c) const noexcept
{
    for(auto&  dst: *this)
    {
      dst.color = c;
      dst.z     = 0;
    }
}


}





