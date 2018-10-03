#ifndef ControlDevice_hpp_was_included
#define ControlDevice_hpp_was_included


#include<cstdint>
#include<cstdio>
#include<vector>
#include"libgbstd/utility.hpp"


namespace gbstd{




struct
point
{
  int  x;
  int  y;

  constexpr point(int  x_=0, int  y_=0) noexcept:
  x(x_),
  y(y_){}


  constexpr bool  operator==(point  rhs) const noexcept
  {
    return(x == rhs.x) &&
          (y == rhs.y);
  }

  constexpr bool  operator!=(point  rhs) const noexcept
  {
    return(x != rhs.x) ||
          (y != rhs.y);
  }

  constexpr point  operator+(point  rhs) const noexcept{return point(x+rhs.x,y+rhs.y);}
  constexpr point  operator-(point  rhs) const noexcept{return point(x-rhs.x,y-rhs.y);}
  constexpr point  operator*(point  rhs) const noexcept{return point(x*rhs.x,y*rhs.y);}
  constexpr point  operator/(point  rhs) const noexcept{return point(x/rhs.x,y/rhs.y);}
  constexpr point  operator%(point  rhs) const noexcept{return point(x%rhs.x,y%rhs.y);}
  constexpr point  operator-() const noexcept{return point(-x,-y);}

  point&  operator+=(point  rhs) noexcept;
  point&  operator-=(point  rhs) noexcept;
  point&  operator*=(point  rhs) noexcept;
  point&  operator/=(point  rhs) noexcept;
  point&  operator%=(point  rhs) noexcept;

  void  print() const noexcept{printf("{x:%d,y:%d}",x,y);}

};




struct
rectangle
{
  int  x;
  int  y;
  int  w;
  int  h;

  static constexpr int  abs(int  i) noexcept{return (i < 0)? -i:i;}

  constexpr rectangle(int  x_=0, int  y_=0, int  w_=0, int  h_=0) noexcept:
  x(x_), y(y_), w(w_), h(h_){}

  constexpr rectangle(point  pt, int  w_=0, int  h_=0) noexcept:
  x(pt.x), y(pt.y), w(w_), h(h_){}

  constexpr rectangle(point  a, point  b) noexcept:
  x(std::min(a.x,b.x)), y(std::min(a.y,b.y)), w(1+abs(a.x-b.x)), h(1+abs(a.y-b.y)){}

  constexpr bool  test_point(const point&  pt) const noexcept
  {
    return((pt.x >= (x  )) &&
           (pt.y >= (y  )) &&
           (pt.x <  (x+w)) &&
           (pt.y <  (y+h)));
  }

};




class
user_input
{
  int  m_state;

  struct flags{
    static constexpr int     up = 0x001;
    static constexpr int   left = 0x002;
    static constexpr int  right = 0x004;
    static constexpr int   down = 0x008;
    static constexpr int      p = 0x010;
    static constexpr int      n = 0x020;
    static constexpr int  shift = 0x040;
    static constexpr int  start = 0x080;

    static constexpr int  mouse_left  = 0x100;
    static constexpr int  mouse_right = 0x200;
  };

  bool   test(int  flag) const noexcept{return m_state&flag;}

  void    set(int  flag) noexcept{m_state |=  flag;}
  void  unset(int  flag) noexcept{m_state &= ~flag;}

public:
  user_input(int  state=0) noexcept: m_state(state){}

  user_input  operator^(user_input  rhs) const noexcept{return user_input(m_state^rhs.m_state);}

  int  get_code() const noexcept{return m_state;}

  bool    test_all() const noexcept{return m_state;}
  bool     test_up() const noexcept{return test(flags::up);}
  bool   test_left() const noexcept{return test(flags::left);}
  bool  test_right() const noexcept{return test(flags::right);}
  bool   test_down() const noexcept{return test(flags::down);}

  bool   test_mouse_left() const noexcept{return test(flags::mouse_left);}
  bool  test_mouse_right() const noexcept{return test(flags::mouse_right);}

  bool  test_p() const noexcept{return test(flags::p);}
  bool  test_n() const noexcept{return test(flags::n);}

  bool  test_shift() const noexcept{return test(flags::shift);}
  bool  test_start() const noexcept{return test(flags::start);}


  void     set_up() noexcept{set(flags::up);}
  void   set_left() noexcept{set(flags::left);}
  void  set_right() noexcept{set(flags::right);}
  void   set_down() noexcept{set(flags::down);}

  void   set_mouse_left() noexcept{set(flags::mouse_left);}
  void  set_mouse_right() noexcept{set(flags::mouse_right);}

  void  set_p() noexcept{set(flags::p);}
  void  set_n() noexcept{set(flags::n);}

  void  set_shift() noexcept{set(flags::shift);}
  void  set_start() noexcept{set(flags::start);}


  void     unset_up() noexcept{unset(flags::up);}
  void   unset_left() noexcept{unset(flags::left);}
  void  unset_right() noexcept{unset(flags::right);}
  void   unset_down() noexcept{unset(flags::down);}

  void   unset_mouse_left() noexcept{unset(flags::mouse_left);}
  void  unset_mouse_right() noexcept{unset(flags::mouse_right);}

  void  unset_p() noexcept{unset(flags::p);}
  void  unset_n() noexcept{unset(flags::n);}

  void  unset_shift() noexcept{unset(flags::shift);}
  void  unset_start() noexcept{unset(flags::start);}

};


extern uint32_t  g_time;

extern std::vector<point>  g_point_buffer;

extern std::vector<uint8_t>  g_dropped_file;

extern bool  g_needed_to_redraw;

extern user_input  g_modified_input;
extern user_input           g_input;

void  update_user_input(user_input  in) noexcept;


}


#endif




