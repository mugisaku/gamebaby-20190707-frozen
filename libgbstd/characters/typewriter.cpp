#include"libgbstd/character.hpp"




namespace gbstd{




typewriter&
typewriter::
assign(const text&  txt) noexcept
{
  m_base_pointer = txt.get_character_pointer(0,0);

  m_text_width = txt.get_width();
  m_width      = txt.get_width();
  m_height     = txt.get_height();

  return *this;
}




void
typewriter::
scroll_up(int  n) noexcept
{
  auto  dst_base = m_base_pointer;
  auto  src_base = get_character_pointer(0,n);

    for(int  y = n;  y < m_height;  ++y)
    {
      auto  dst = dst_base                ;
                  dst_base += m_text_width;

      auto  src = src_base                ;
                  src_base += m_text_width;

        for(int  x = 0;  x < m_width;  ++x)
        {
          *dst++ = *src++;
        }
    }
}


point
typewriter::
overwrite(const character*  s, int  l, point  pos) const noexcept
{
  int  x = pos.x;
  int  y = pos.y;

    while(l--)
    {
        if(x >= m_width)
        {
            if(++y >= m_height)
            {
              break;
            }


          x = 0;
        }


      auto  c = *s++;

        if(c.unicode == '\n')
        {
            if(++y >= m_height)
            {
              break;
            }


          x = 0;
        }

      else
        {
          *get_character_pointer(x,y) = c;

          ++x;
        }
    }


  return point(x,y);
}


point
typewriter::
overwrite(const char*  s, gbstd::color  color, point  pos) const noexcept
{
  auto  l = u8slen(s);

  char16_t  us[l+1];

  char16_t*  p = us;

  utf8_decoder  dec(s);

    while(dec)
    {
      *p++ = dec();
    }


  *p = 0;

  return overwrite(us,color,pos);
}


point
typewriter::
overwrite(const char16_t*  s, gbstd::color  color, point  pos) const noexcept
{
    while(*s)
    {
      character  c;

      c.unicode =  *s++;
      c.color   = color;

      pos = overwrite(&c,1,pos);
    }


  return pos;
}



void
typewriter::
fill(character  c) const noexcept
{
  auto  dst_base = m_base_pointer;

    for(int  y = 0;  y < m_height;  ++y)
    {
      auto  dst = dst_base                ;
                  dst_base += m_text_width;

        for(int  x = 0;  x < m_width;  ++x)
        {
          *dst++ = c;
        }
    }
}




}




