#ifndef gbstd_text_HPP
#define gbstd_text_HPP


#include"libgbstd/control.hpp"
#include<string>
#include<cstring>




namespace gbstd{




constexpr int  g_font_width  =  8;
constexpr int  g_font_height = 16;

struct
glyph
{
  char16_t  unicode;

  uint8_t  data[g_font_height];

};


const glyph&  get_glyph(char16_t  c) noexcept;




class
utf8_decoder
{
  const char*  m_pointer=nullptr;

public:
  constexpr utf8_decoder(              ) noexcept{}
  constexpr utf8_decoder(const char*  s) noexcept: m_pointer(s){}

  utf8_decoder&  operator=(const char*  s) noexcept{m_pointer = s;  return *this;}

  operator bool() const noexcept{return *m_pointer;}

  char32_t  operator()() noexcept;

  const char*  get_pointer() const noexcept{return m_pointer;}

};


size_t  u8slen(const char*  s) noexcept;


struct
utf8_encoder
{
  char  codes[8];

  utf8_encoder(char32_t  c=0) noexcept{(*this)(c);}

  utf8_encoder&  operator()(char32_t  c) noexcept;

};


std::u16string  make_u16string(std::string_view  sv) noexcept;


void  print(std::u16string_view  sv) noexcept;


class
text
{
private:
  struct line{
    static constexpr int  m_buffer_length = 80;

    char16_t  m_buffer[m_buffer_length];

    int  m_length;

    line*  m_next;

    std::u16string  get_view(      ) const noexcept{return std::u16string(m_buffer,m_length);}
    std::u16string  get_view(int  l) const noexcept{return std::u16string(m_buffer,l);}

    const char16_t*  begin() const noexcept{return m_buffer         ;}
    const char16_t*    end() const noexcept{return m_buffer+m_length;}

  };

public:
  class iterator{
    const line*  m_pointer=nullptr;

  public:
    iterator(const line*  ptr) noexcept: m_pointer(ptr){}

    const line&  operator *() const noexcept{return *m_pointer;}
    const line*  operator->() const noexcept{return  m_pointer;}

    operator bool() const noexcept{return m_pointer;}

    bool  operator==(iterator  rhs) noexcept{return m_pointer == rhs.m_pointer;}
    bool  operator!=(iterator  rhs) noexcept{return m_pointer != rhs.m_pointer;}

    iterator&  operator++() noexcept{  m_pointer = m_pointer->m_next;  return *this;}

    iterator  operator+(int  n) const noexcept
    {
      auto  ptr = m_pointer;

        while(n--)
        {
          ptr = ptr->m_next;
        }


      return iterator(ptr);
    }

    iterator&  operator+=(int  n) noexcept
    {
        while(n--)
        {
          m_pointer = m_pointer->m_next;
        }


      return *this;
    }

  };


private:
  line*     m_top_pointer=nullptr;
  line*  m_bottom_pointer=nullptr;

  int  m_number_of_lines=0;

  static line*  m_stock_pointer;

  line*  new_line() noexcept;

public:
  text() noexcept{}
  text(const text&   rhs) noexcept{assign(rhs);}
  text(      text&&  rhs) noexcept{assign(std::move(rhs));}
 ~text(){clear();}

  text&  operator=(const text&   rhs) noexcept{return assign(rhs);}
  text&  operator=(      text&&  rhs) noexcept{return assign(std::move(rhs));}

  text&  assign(const text&   rhs) noexcept;
  text&  assign(      text&&  rhs) noexcept;

  text&  clear() noexcept;

  int  get_number_of_lines() const noexcept{return m_number_of_lines;}

  text&  push(std::string_view     sv) noexcept;
  text&  push(std::u16string_view  sv) noexcept;
  text&   pop() noexcept;

  iterator  bottom() const noexcept{return iterator(m_bottom_pointer);}

  iterator  begin() const noexcept{return iterator(m_top_pointer);}
  iterator    end() const noexcept{return iterator(nullptr);}

};




}




#endif




