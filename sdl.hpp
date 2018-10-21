#ifndef sdl_hpp_was_inclueded
#define sdl_hpp_was_inclueded


#include"libgbstd/image.hpp"
#include"libgbstd/control.hpp"
#include<SDL.h>


namespace sdl{


void  update_screen(const gbstd::canvas&  cv) noexcept;

void  quit() noexcept;

void  init(                int  w, int  h, double  scale=1) noexcept;
void  init(int  x, int  y, int  w, int  h, double  scale=1) noexcept;

void  resize_screen(int  w, int  h) noexcept;

gbstd::canvas  make_screen_canvas() noexcept;

void  update_control() noexcept;


}




#endif




