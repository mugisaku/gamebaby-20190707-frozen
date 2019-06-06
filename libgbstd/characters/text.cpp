#include"libgbstd/character.hpp"




namespace gbstd{




text::line*
text::
m_stock_pointer;




text&
text::
assign(const text&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

        for(auto&  ln: rhs)
        {
          push(ln.get_view());
        }
    }


  return *this;
}


text&
text::
assign(text&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_top_pointer    ,rhs.m_top_pointer);
      std::swap(m_bottom_pointer ,rhs.m_bottom_pointer);
      std::swap(m_number_of_lines,rhs.m_number_of_lines);
    }


  return *this;
}




text&
text::
clear() noexcept
{
  auto  ptr = m_top_pointer;

  m_top_pointer    = nullptr;
  m_bottom_pointer = nullptr;

    while(ptr)
    {
      auto  next = ptr->m_next;
                   ptr->m_next = m_stock_pointer      ;
                                 m_stock_pointer = ptr;

      ptr = next;
    }


  m_number_of_lines = 0;

  return *this;
}


text::line*
text::
new_line() noexcept
{
    if(!m_stock_pointer)
    {
      m_stock_pointer = new line;

      m_stock_pointer->m_next = nullptr;
    }


  auto  ln = m_stock_pointer                          ;
             m_stock_pointer = m_stock_pointer->m_next;

    if(m_bottom_pointer)
    {
      m_bottom_pointer->m_next = ln;
    }

  else
    {
      m_top_pointer = ln;
    }


  m_bottom_pointer = ln;

  ln->m_length = 0;

  ++m_number_of_lines;

  return ln;
}


text&
text::
push(std::string_view  sv) noexcept
{
  auto  ln = new_line();

  char16_t*  p = ln->m_buffer;

  utf8_decoder  dec(sv.data());

    while(dec)
    {
      *p++ = dec();

      ++ln->m_length;
    }


  *p = 0;

  return *this;
}


text&
text::
push(std::u16string_view  sv) noexcept
{
  auto  ln = new_line();

  char16_t*  p = ln->m_buffer;

    for(auto  c: sv)
    {
      *p++ = c;

      ++ln->m_length;
    }


  *p = 0;

  return *this;
}


text&
text::
pop() noexcept
{
    if(m_top_pointer)
    {
      auto  ln = m_top_pointer                        ;
                 m_top_pointer = m_top_pointer->m_next;

      ln->m_next = m_stock_pointer     ;
                   m_stock_pointer = ln;

        if(!--m_number_of_lines)
        {
          m_bottom_pointer = nullptr;
        }
    }


  return *this;
}




}




