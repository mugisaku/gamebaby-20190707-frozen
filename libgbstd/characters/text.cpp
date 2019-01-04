#include"libgbstd/character.hpp"




namespace gbstd{




void
text::
resize(int  w, int  h) noexcept
{
    if(!w || !h)
    {
      delete[] m_data          ;
               m_data = nullptr;

      m_width  = 0;
      m_height = 0;

      return;
    }


  
  m_width  = w;
  m_height = h;

  delete[] m_data                     ;
           m_data = new character[w*h];
}


void
text::
fill(character  c) noexcept
{
  auto  p = m_data;

  auto  end = m_data+(m_width*m_height);

    while(p != end)
    {
      *p++ = c;
    }
}


}




