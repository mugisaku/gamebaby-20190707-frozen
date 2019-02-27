#ifndef sdl_hpp_was_inclueded
#define sdl_hpp_was_inclueded


#include"libgbstd/image.hpp"
#include"libgbstd/control.hpp"


namespace sdl{


void  update_screen(const gbstd::canvas&  cv) noexcept;

void  delay(uint32_t  ms) noexcept;

void  quit() noexcept;

void  init(                int  w, int  h, double  scale=1) noexcept;
void  init(int  x, int  y, int  w, int  h, double  scale=1) noexcept;

void  init_sound() noexcept;
void  quit_sound() noexcept;

void  start_sound() noexcept;
void   stop_sound() noexcept;

void  beep(uint32_t  ms) noexcept;

void  resize_screen(int  w, int  h) noexcept;

void  screenshot() noexcept;

gbstd::canvas  make_screen_canvas() noexcept;

void  update_control() noexcept;


}




#endif




