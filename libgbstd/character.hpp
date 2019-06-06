#ifndef GMBB_CUI_SCREEN_HPP
#define GMBB_CUI_SCREEN_HPP


#include"libgbstd/control.hpp"
#include<string>
#include<queue>
#include<initializer_list>




namespace gbstd{




struct
color
{
  uint16_t  code;

  static constexpr int  masked(int  v) noexcept{return v&7;}

  constexpr color(int  v=0) noexcept: code((v&01000)? color(v>>6,v>>3,v).code:0){}
  constexpr color(int  r7, int  g7, int  b7) noexcept: code(0x8000|((masked(r7)<<2)<<10)|
                                                                   ((masked(g7)<<2)<< 5)|
                                                                   ((masked(b7)<<2)    )){}

  constexpr operator bool() const noexcept{return code>>15;}

  constexpr bool  operator==(const color&  rhs) const noexcept{return code == rhs.code;}
  constexpr bool  operator!=(const color&  rhs) const noexcept{return code != rhs.code;}

  constexpr int  get_r7() const noexcept{return masked(((code>>10)>>2));}
  constexpr int  get_g7() const noexcept{return masked(((code>> 5)>>2));}
  constexpr int  get_b7() const noexcept{return masked(((code    )>>2));}

  constexpr int  get_r255() const noexcept{return (get_r7()<<5)|0b11111;}
  constexpr int  get_g255() const noexcept{return (get_g7()<<5)|0b11111;}
  constexpr int  get_b255() const noexcept{return (get_b7()<<5)|0b11111;}

  void  print() const noexcept{printf("0%d%d%d%d",(*this? 1:0),get_r7(),get_g7(),get_b7());}

};


namespace colors{
constexpr color        null = 0;
constexpr color       white = color(7,7,7);
constexpr color       black = color(0,0,0);
constexpr color        gray = color(3,3,3);
constexpr color  light_gray = color(5,5,5);
constexpr color   dark_gray = color(1,1,1);
constexpr color         red = color(7,0,0);
constexpr color       green = color(0,7,0);
constexpr color        blue = color(0,0,7);
constexpr color      yellow = color(7,7,0);
}




constexpr int  g_font_width  =  8;
constexpr int  g_font_height = 16;

struct
glyph
{
  char16_t  unicode;

  uint16_t  data[g_font_height];

};


const glyph*  get_glyph(char16_t  c) noexcept;




class
character_color
{
  color  m_colors[4];

public:
  constexpr character_color() noexcept{}
  constexpr character_color(color  a                              ) noexcept: m_colors{colors::null,a}{}
  constexpr character_color(color  a, color  b                    ) noexcept: m_colors{a,b}{}
  constexpr character_color(color  a, color  b, color  c          ) noexcept: m_colors{colors::null,a,b,c}{}
  constexpr character_color(color  a, color  b, color  c, color  d) noexcept: m_colors{a,b,c,d}{}

        color&  operator[](int  i)       noexcept{return m_colors[i];}
  const color&  operator[](int  i) const noexcept{return m_colors[i];}

};


struct
character
{
  character_color  color;

  char16_t  unicode;

  constexpr character() noexcept: unicode(0){}

};




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




