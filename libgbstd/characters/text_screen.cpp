#include"libgbstd/character.hpp"




namespace gbstd{




void
text_screen::
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
text_screen::
fill(const character&  c) noexcept
{
}


}




