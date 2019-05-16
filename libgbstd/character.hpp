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


std::u16string  make_u16string(const std::string&  s) noexcept;


void  print(std::u16string_view  sv) noexcept;


class
text
{
  character*  m_data=nullptr;

  int  m_width =0;
  int  m_height=0;

public:
  text(              ) noexcept{}
  text(int  w, int  h) noexcept{resize(w,h);}
 ~text(){resize(0,0);}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  int  get_image_width()  const noexcept{return g_font_width*m_width;}
  int  get_image_height() const noexcept{return g_font_height*m_height;}

  void  resize(int  w, int  h) noexcept;

  character*  get_character_pointer(int  x, int  y) const noexcept{return m_data+(m_width*y)+x;}

  void  fill(character  c=character()) noexcept;

};


class
typewriter
{
  character*  m_base_pointer=nullptr;

  int  m_text_width=0;

  int  m_width =0;
  int  m_height=0;

  point  m_cursor_pos;

  std::queue<character>  m_queue;

public:
  typewriter() noexcept{}
  typewriter(const text&  txt) noexcept{assign(txt);}

  typewriter&  operator=(const text&  txt) noexcept{return assign(txt);}
  typewriter&     assign(const text&  txt) noexcept;

  int  get_text_width()  const noexcept{return m_text_width;}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  int  get_image_width()  const noexcept{return g_font_width*m_width;}
  int  get_image_height() const noexcept{return g_font_height*m_height;}

  character*  get_character_pointer(int  x, int  y) const noexcept{return m_base_pointer+(m_text_width*y)+x;}

  bool  has_cursor_reached_right()  const noexcept{return m_cursor_pos.x == (m_width -1);}
  bool  has_cursor_reached_bottom() const noexcept{return m_cursor_pos.y == (m_height-1);}

  void  newline() noexcept;

  const std::queue<character>&  get_queue() const noexcept{return m_queue;}

  void  clear_queue() noexcept{m_queue = std::queue<character>();}

  point  get_cursor_position() const noexcept{return m_cursor_pos;}

  void  set_cursor_position(point  pos) noexcept{m_cursor_pos  = pos;}
  void  add_cursor_position(point  pos) noexcept{m_cursor_pos += pos;}

  void  clear_line(      ) noexcept{clear_line(m_cursor_pos.y);}
  void  clear_line(int  n) noexcept;

  void  scroll_up() noexcept;

  operator bool() const noexcept{return m_queue.size();}

  void  push(const character*  s, int  l) noexcept;

  void  push(const char*      s, gbstd::color  color) noexcept;
  void  push(const char16_t*  s, gbstd::color  color) noexcept;

  void  pump() noexcept;

  void  fill(character  c=character()) const noexcept;

};




}




#endif




