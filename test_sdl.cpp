#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/value.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/window.hpp"
#include"libgbstd/windows/component.hpp"
#include"libsubiso/subiso.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas
g_screen_canvas;


subiso::space
g_space;


window
g_window;


class
shell: public widget
{
  subiso::space_handler  m_handler;

  point  m_last_point;

  const subiso::plane_reference_stack*  m_last_stack=nullptr;
  subiso::plane*                        m_last_plane=nullptr;

  bool  m_lock=false;

  void  process(subiso::plane_reference&  ref) noexcept
  {
    auto  pl = ref.get_plane();

      if(pl)
      {
          if(g_input.test_mouse_right())
          {
            pl->m_box->be_null();

            m_lock = true;
          }

        else
          if(g_input.test_mouse_left())
          {
              if(m_last_plane->is_top() && ref.get_box()->m_up_box)
              {
                ref.get_box()->m_up_box->be_earth();
              }


            m_lock = true;
          }
      }

    else
      if(g_input.test_mouse_left())
      {
        auto  box = ((&ref)-1)->get_box();

          if(box)
          {
            box->be_earth();

            m_lock = true;
          }
      }
  }

public:
  shell(subiso::space&  sp) noexcept{assign(sp);}

  subiso::space_handler&  get_handler() noexcept{return m_handler;}

  shell&  assign(subiso::space&  sp) noexcept
  {
    m_handler.assign(sp);

    request_reform();

    return *this;
  }

  void  process_before_reform() noexcept override
  {
    auto&  fb = m_handler.get_stack_map(subiso::direction::front);
    auto&  lr = m_handler.get_stack_map(subiso::direction::left );

    int  w = std::max(fb.get_image_width() ,lr.get_image_width() );
    int  h = std::max(fb.get_image_height(),lr.get_image_height());

    set_content_width( w*4);
    set_content_height(h);
  }

  void  do_on_mouse_act(point  mouse_pos) noexcept override
  {
    int  x = mouse_pos.x/subiso::g_plane_size;
    int  y = mouse_pos.y/subiso::g_plane_size;

    auto&  refstack = m_handler.get_stack_map().get_stack(x,y);

    m_last_stack = &refstack;

    auto&  ref = refstack.get_current();

      if((x != m_last_point.x) ||
         (y != m_last_point.y))
      {
        m_lock = false;

        m_last_point.x = x;
        m_last_point.y = y;
      }


      if(!g_input.test_mouse_left() &&
         !g_input.test_mouse_right())
      {
        m_lock = false;
      }


    m_last_plane = ref.get_plane();

      if(!m_lock)
      {
        process(ref);

        request_redraw();
      }
  }

  void  render(const canvas&  cv) noexcept override
  {
    cv.fill(color());

    m_handler.render2(cv);

      if(m_last_plane)
      {
        string_form  sf;

        auto&  box = *m_last_plane->m_box;

        auto  i = box.m_index;

  //      auto  flags = g_last_box->m_top_plane.get_flags();

        static character_color  chcolor(colors::black,colors::white);

          if(0 && m_last_stack)
          {
            cv.draw_string(chcolor,sf("%3d",m_last_stack->get_current().get_image_z_base()),0,0);
          }
  /*
        g_screen_canvas.draw_string(chcolor,sf("%s%s%s%s",flags&top_flag? "T":"-",
                                                          flags&bottom_flag? "B":"-",
                                                          flags&left_flag? "L":"-",
                                                          flags&right_flag? "R":"-"),0,48);
  */
      }
  }

  void  step() noexcept
  {
    m_handler.step();

    request_redraw();
  }

};


shell*
g_shell;


void
main_loop() noexcept
{
  sdl::update_control();

  g_shell->step();

  g_window.do_total_reform_if_necessary();

    if(g_point_buffer.size())
    {
        for(auto&  pt: g_point_buffer)
        {
          g_window.process_user_input(pt);
        }
    }


    if(g_shell->get_handler().test_dirty_flag())
    {
      g_shell->request_redraw();
    }


    if(g_window.redraw_if_necessary() || g_needed_to_redraw)
    {
      g_screen_canvas.fill(color());

      g_screen_canvas.draw_canvas(g_window.get_content_image(),0,0);

      sdl::update_screen(g_screen_canvas);
    }
}


}




int
main(int  argc, char**  argv)
{
#ifdef __EMSCRIPTEN__
  set_description("<pre>"
/*
                  "正方形の頂面と側面を等尺で、\n"
                  "二次元にレンダリングするプログラム\n"
                  "\n"
                  "適切な繋ぎ目を持つ画像に、自動で切り替わる\n"
                  "\n"
                  "\n"
*/
                  "カーソルキーで移動（XY値を変化）\n"
                  "Shiftキーを押しながら上下を押すと、Z値を変化\n"
//                  "マウスの左ボタンを押すと、ボックスを置く\n"
//                  "　　　　右ボタンを押すと、ボックスを消す\n"
                  "</pre>");

  show_github_link();
#endif


  g_space.resize(5,5,5);

    for(int  y = 0;  y < g_space.get_y_length();  ++y){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,y,0).m_kind = subiso::box::kind::earth;
    }}


  g_space.get_box(1,0,1).m_kind = subiso::box::kind::earth;
  g_space.get_box(1,1,1).m_kind = subiso::box::kind::earth;

  g_shell = new shell(g_space);

  g_window.set_root_widget(g_shell);

  sdl::init(g_window.get_width(),g_window.get_height());

  g_screen_canvas = sdl::make_screen_canvas();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




