#ifndef GMBB_CUI_SCREEN_HPP
#define GMBB_CUI_SCREEN_HPP


#include"libgbstd/control.hpp"
#include<string>
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


std::u16string  make_u16string(const std::string&  s) noexcept;


class
text_buffer
{
  static constexpr size_t  m_length_limit = 512;

  character  m_data[m_length_limit];

  int   m_i_pos=0;
  int   m_o_pos=0;

  int   m_length=0;

public:
  operator bool() const noexcept{return m_length;}

  int  get_length() const noexcept{return m_length;}

  void  clear() noexcept;

  void  push(const char*      s, bool  with_newline=true);
  void  push(const char16_t*  s, bool  with_newline=true);

  const character&  pop() noexcept;

};


class
text_screen
{
  character*  m_data=nullptr;

  int  m_width =0;
  int  m_height=0;

public:
  text_screen(              ) noexcept{}
  text_screen(int  w, int  h) noexcept{resize(w,h);}
 ~text_screen(){resize(0,0);}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  resize(int  w, int  h) noexcept;

  character*  get_character_pointer(int  x, int  y) const noexcept{return m_data+(m_width*y)+x;}

  void  fill(const character&  c) noexcept;

};


class
text_cursor
{
  text_screen*  m_screen=nullptr;

  point  m_point;

public:
  text_cursor() noexcept{}
  text_cursor(text_screen&  scr) noexcept: m_screen(&scr){}

  operator bool() const noexcept{return (m_point.x < m_screen->get_width()) &&
                                        (m_point.y < m_screen->get_height());}

  character&  operator*() const noexcept{return *m_screen->get_character_pointer(m_point.x,m_point.y);}

  text_cursor&  operator++() noexcept;
  text_cursor  operator++(int) noexcept;
  text_cursor&  operator+=(point  pt) noexcept;

  void  reset(int  x, int  y) noexcept{m_point = point(x,y);}

};




}




#endif




