#ifndef GMBB_GUI_IMAGE_HPP
#define GMBB_GUI_IMAGE_HPP


#include"libgbstd/character.hpp"




namespace gbstd{




class
liner
{
  static constexpr int  shift_amount = 16;

  int  m_distance;

  int  m_x_add_amount;
  int  m_y_add_amount;

  int  m_x;
  int  m_y;

public:
  liner(int  x0=0, int  y0=0, int  x1=0, int  y1=0) noexcept{reset(x0,y0,x1,y1);}
  liner(point  a, point  b) noexcept{reset(a,b);}

  void  reset(int  x0=0, int  y0=0, int  x1=0, int  y1=0) noexcept;
  void  reset(point  a, point  b) noexcept{reset(a.x,a.y,b.x,b.y);}

  int  get_distance() const noexcept{return m_distance;}

  int  get_x() const noexcept{return m_x>>shift_amount;}
  int  get_y() const noexcept{return m_y>>shift_amount;}

  void  step() noexcept;

};




struct
pixel
{
  gbstd::color  color=0;

  int16_t  z=0;

  constexpr pixel(gbstd::color  c=gbstd::color(), int16_t  z_=0) noexcept:
  color(c), z(z_){}

  constexpr operator gbstd::color() const noexcept{return color;}

};




class
image
{
  int  m_width =0;
  int  m_height=0;

  pixel*  m_pixels=nullptr;

public:
  image() noexcept{}
  image(const image&   rhs) noexcept{assign(rhs);}
  image(      image&&  rhs) noexcept{assign(std::move(rhs));}
  image(int  w, int  h, std::initializer_list<int>  ls) noexcept;
  image(int  w, int  h, std::initializer_list<color>  ls={}) noexcept;
 ~image(){resize(0,0);}


  image&  operator=(const image&   rhs) noexcept{return assign(rhs);}
  image&  operator=(      image&&  rhs) noexcept{return assign(std::move(rhs));}

  image&  assign(const image&   rhs) noexcept;
  image&  assign(      image&&  rhs) noexcept;


  void  resize(int  w, int  h) noexcept;

  std::vector<uint8_t>  make_png_stream(                   ) const noexcept;
  void                  read_png_stream(const uint8_t*  ptr)       noexcept;

  void  load_png(const char*  path) noexcept;

  int   get_width() const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  rectangle  get_rectangle() const noexcept{return rectangle(0,0,m_width,m_height);}

  pixel*  begin() const noexcept{return m_pixels;}
  pixel*    end() const noexcept{return m_pixels+(m_width*m_height);}

  pixel*  get_pixel_pointer(int  x, int  y) const noexcept{return m_pixels+(m_width*y)+x;}

  void  fill(color  c) const noexcept;

};


struct
color_modifier
{
  int  r;
  int  g;
  int  b;

};


struct
sprite
{
  const gbstd::image*  image;

  gbstd::rectangle  rectangle;

};




class
canvas
{
  pixel*  m_pointer=nullptr;

  int  m_image_width=0;

  int  m_width =0;
  int  m_height=0;

public:
  canvas() noexcept{}
  canvas(const image&  img) noexcept{assign(img);}
  canvas(const image&  img, int  x, int  y, int  w, int  h) noexcept{assign(img,x,y,w,h);}
  canvas(const image&  img, const rectangle&  rect) noexcept{assign(img,rect);}
  canvas(const canvas&  cv, int  x, int  y, int  w, int  h) noexcept{assign(cv,x,y,w,h);}
  canvas(const canvas&  cv, const rectangle&  rect) noexcept{assign(cv,rect);}

  operator bool() const noexcept{return m_pointer;}

  canvas&  assign(const image&  img                                ) noexcept;
  canvas&  assign(const image&  img, int  x, int  y, int  w, int  h) noexcept;
  canvas&  assign(const image&  img, const rectangle&  rect) noexcept{return assign(img,rect.x,rect.y,rect.w,rect.h);}
  canvas&  assign(const canvas&  cv, int  x, int  y, int  w, int  h) noexcept;
  canvas&  assign(const canvas&  cv, const rectangle&  rect) noexcept{return assign(cv,rect.x,rect.y,rect.w,rect.h);}

  int   get_width() const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int  get_image_width() const noexcept{return m_image_width;}

  pixel*  get_pixel_pointer(int  x, int  y) const noexcept{return m_pointer+(m_image_width*y)+x;}

  void  fill(color  c                                ) const noexcept;
  void  fill(color  c, int  x, int  y, int  w, int  h) const noexcept;

  void  add_rgb(color_modifier  mod                                ) const noexcept;
  void  add_rgb(color_modifier  mod, int  x, int  y, int  w, int  h) const noexcept;

  void  reverse_rgb(                              ) const noexcept;
  void  reverse_rgb(int  x, int  y, int  w, int  h) const noexcept;


  void  draw_dot(       color  c, int  x, int  y) const noexcept;
  void  draw_dot_safely(color  c, int  x, int  y) const noexcept;

  void  draw_dot(       color  c, point  pt) const noexcept{draw_dot(       c,pt.x,pt.y);}
  void  draw_dot_safely(color  c, point  pt) const noexcept{draw_dot_safely(c,pt.x,pt.y);}

  void  draw_vline(       color  c, int  x, int  y, int  l) const noexcept;
  void  draw_vline_safely(color  c, int  x, int  y, int  l) const noexcept;
  void  draw_vline(       color  c, point  pt, int  l) const noexcept{draw_vline(c,pt.x,pt.y);}
  void  draw_vline_safely(color  c, point  pt, int  l) const noexcept{draw_vline_safely(c,pt.x,pt.y);}

  void  draw_hline(       color  c, int  x, int  y, int  l) const noexcept;
  void  draw_hline_safely(color  c, int  x, int  y, int  l) const noexcept;

  void  draw_hline(       color  c, point  pt, int  l) const noexcept{draw_hline(c,pt.x,pt.y);}
  void  draw_hline_safely(color  c, point  pt, int  l) const noexcept{draw_hline_safely(c,pt.x,pt.y);}

  void  draw_line(       color   c, int  x0, int  y0, int  x1, int  y1) const noexcept;
  void  draw_line_safely(color   c, int  x0, int  y0, int  x1, int  y1) const noexcept;
  void  draw_line(       color   c, point  a, point  b) const noexcept{draw_line(c,a.x,a.y,b.x,b.y);}
  void  draw_line_safely(color   c, point  a, point  b) const noexcept{draw_line_safely(c,a.x,a.y,b.x,b.y);}

  void  draw_rectangle(       color  c, int  x, int  y, int  w, int  h) const noexcept;
  void  draw_rectangle_safely(color  c, int  x, int  y, int  w, int  h) const noexcept;
  void  fill_rectangle(       color  c, int  x, int  y, int  w, int  h) const noexcept;
  void  fill_rectangle_safely(color  c, int  x, int  y, int  w, int  h) const noexcept;
  void  draw_rectangle(       color  c, const rectangle&  rect) const noexcept{draw_rectangle(c,rect.x,rect.y,rect.w,rect.h);}
  void  draw_rectangle_safely(color  c, const rectangle&  rect) const noexcept{draw_rectangle_safely(c,rect.x,rect.y,rect.w,rect.h);}
  void  fill_rectangle(       color  c, const rectangle&  rect) const noexcept{fill_rectangle(c,rect.x,rect.y,rect.w,rect.h);}
  void  fill_rectangle_safely(color  c, const rectangle&  rect) const noexcept{fill_rectangle_safely(c,rect.x,rect.y,rect.w,rect.h);}

  void  draw_double_rectangle(color  out, color  in, int  x, int  y, int  w, int  h) const noexcept;
  void  draw_double_rectangle(color  out, color  in, const rectangle&  rect) const noexcept;

  void  draw_stripe_rectangle(color  first, color  second, int  interval, int  x, int  y, int  w, int  h) const noexcept;
  void  draw_stripe_rectangle(color  first, color  second, int  interval, const rectangle&  rect) const noexcept;

  void  draw_glyph(       const character_color&  chr_color, const glyph&  gl, int  x, int  y) const noexcept;
  void  draw_glyph_safely(const character_color&  chr_color, const glyph&  gl, int  x, int  y) const noexcept;

  void  draw_character(const character&  c, int  x, int  y) const noexcept;
  void  draw_character(const character&  c, point  pt) const noexcept{draw_character(c,pt.x,pt.y);}

  void  draw_character_safely(const character&  c, int  x, int  y) const noexcept;
  void  draw_character_safely(const character&  c, point  pt) const noexcept{draw_character_safely(c,pt.x,pt.y);}

  void  draw_string(const character_color&  color, const char*      s, int  x, int  y) const noexcept;
  void  draw_string(const character_color&  color, const char16_t*  s, int  x, int  y) const noexcept;
  void  draw_string(const character_color&  color, const char*      s, point  pt) const noexcept{draw_string(color,s,pt.x,pt.y);}
  void  draw_string(const character_color&  color, const char16_t*  s, point  pt) const noexcept{draw_string(color,s,pt.x,pt.y);}

  void  draw_string_safely(const character_color&  color, const char*      s, int  x, int  y) const noexcept;
  void  draw_string_safely(const character_color&  color, const char16_t*  s, int  x, int  y) const noexcept;
  void  draw_string_safely(const character_color&  color, const char*      s, point  pt) const noexcept{draw_string_safely(color,s,pt.x,pt.y);}
  void  draw_string_safely(const character_color&  color, const char16_t*  s, point  pt) const noexcept{draw_string_safely(color,s,pt.x,pt.y);}

  void  draw_sprite(const sprite&  spr, int  x, int  y) const noexcept;
  void  draw_sprite(const sprite&  spr, point  pt) const noexcept{draw_sprite(spr,pt.x,pt.y);}

  void  draw_canvas(const canvas&  cv, int  x, int  y) const noexcept;
  void  draw_canvas(const canvas&  cv, point  pt) const noexcept{draw_canvas(cv,pt.x,pt.y);}

  void  copy_canvas(const canvas&  cv, int  x, int  y) const noexcept;
  void  copy_canvas(const canvas&  cv, point  pt) const noexcept{copy_canvas(cv,pt.x,pt.y);}

};




}




#endif




