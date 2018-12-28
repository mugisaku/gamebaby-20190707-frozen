#include"libgbstd/character.hpp"
#include<cctype>




namespace gbstd{




typewriter&
typewriter::
assign(const text&  txt) noexcept
{
  m_base_pointer = txt.get_character_pointer(0,0);

  m_text_width = txt.get_width();
  m_width      = txt.get_width();
  m_height     = txt.get_height();

  m_cursor_pos = {0,0};

  clear_queue();


  return *this;
}




void
typewriter::
newline() noexcept
{
  m_cursor_pos.x = 0;

    if(!has_cursor_reached_bottom())
    {
      ++m_cursor_pos.y;
    }
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


void
typewriter::
clear_line(int  n) noexcept
{
  auto  dst = get_character_pointer(0,n);

    for(int  x = 0;  x < m_width;  ++x)
    {
      *dst++ = character();
    }
}




void
typewriter::
push(const character*  s, int  l) noexcept
{
    while(l--)
    {
      m_queue.emplace(*s++);
    }
}


void
typewriter::
push(const char*  s, gbstd::color  color) noexcept
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

  push(us,color);
}


void
typewriter::
push(const char16_t*  s, gbstd::color  color) noexcept
{
    while(*s)
    {
      character  c;

      c.unicode =  *s++;
      c.color   = color;

      push(&c,1);
    }
}


void
typewriter::
pump() noexcept
{
  auto  c = m_queue.front();

  m_queue.pop();

    if(c.unicode == '\n')
    {
      newline();
    }

  else
    if(!iscntrl(c.unicode))
    {
      *get_character_pointer(m_cursor_pos.x,m_cursor_pos.y) = c;

        if(!has_cursor_reached_right())
        {
          ++m_cursor_pos.x;
        }
    }
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




