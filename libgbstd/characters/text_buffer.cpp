#include"libgbstd/character.hpp"
#include<cctype>




namespace gbstd{




namespace{
constexpr bool  islower(char  c) noexcept{return((c >= 'a') && (c <= 'z'));}
constexpr bool  isupper(char  c) noexcept{return((c >= 'A') && (c <= 'Z'));}
constexpr bool  isalpha(char  c) noexcept{return(isupper(c) || islower(c));}
constexpr bool  isdigit(char  c) noexcept{return((c >= '0') && (c <= '9'));}
constexpr bool  isalnum(char  c) noexcept{return(isalpha(c) || isdigit(c));}

constexpr bool  isident0(char  c) noexcept{return(isalpha(c) || (c == '_'));}
constexpr bool  isidentn(char  c) noexcept{return(isalnum(c) || (c == '_'));}
}




void
text_buffer::
clear() noexcept
{
  m_i_pos  = 0;
  m_o_pos  = 0;
  m_length = 0;
}


void
text_buffer::
push(const char*  s, bool  with_newline)
{
  utf8_decoder  dec(s);

    while(dec)
    {
        if(m_length >= m_length_limit)
        {
          printf("character_queue push error: バッファがいっぱいです\n");

          break;
        }


      auto  unicode = dec();

        if(!unicode)
        {
          break;
        }


      auto&  ch = m_data[m_i_pos++];

      ++m_length;

      ch.unicode = unicode;
    }


    if(with_newline)
    {
        if(m_length < m_length_limit)
        {
          m_data[m_i_pos++].unicode = '\n';
        }
    }
}


const character&
text_buffer::
pop() noexcept
{
  auto  i = m_o_pos++;

  --m_length;

  return m_data[i];
}




}




