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

  subiso::plane*  m_last_plane=nullptr;

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

  void  revolve_to_left() noexcept
  {
    auto  dir = m_handler.get_direction();

    dir = (dir == subiso::direction::front)? subiso::direction::left
         :(dir == subiso::direction::left)?  subiso::direction::back
         :(dir == subiso::direction::back)?  subiso::direction::right
         :                                   subiso::direction::front;

    m_handler.set_direction(dir);


    request_redraw();
  }

  void  revolve_to_right() noexcept
  {
    auto  dir = m_handler.get_direction();

    dir = (dir == subiso::direction::front)? subiso::direction::left
         :(dir == subiso::direction::left)?  subiso::direction::back
         :(dir == subiso::direction::back)?  subiso::direction::right
         :                                   subiso::direction::front;

    m_handler.set_direction(dir);


    request_redraw();
  }

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

    set_content_width( w);
    set_content_height(h);
  }

  void  do_on_mouse_act(point  mouse_pos) noexcept override
  {
    int  x = mouse_pos.x/subiso::g_plane_size;
    int  y = mouse_pos.y/subiso::g_plane_size;

    auto&  ref = m_handler.get_stack_map().get_stack(x,y).get_current();

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

    m_handler.render(cv);

      if(m_last_plane)
      {
        string_form  sf;

        auto&  box = *m_last_plane->m_box;

        auto  i = box.m_index;

  //      auto  flags = g_last_box->m_top_plane.get_flags();

        static character_color  chcolor(colors::black,colors::white);

        cv.draw_string(chcolor,sf("X:%3d",i.x),0, 0);
        cv.draw_string(chcolor,sf("Y:%3d",i.y),0,16);
        cv.draw_string(chcolor,sf("Z:%3d",i.z),0,32);
  /*
        g_screen_canvas.draw_string(chcolor,sf("%s%s%s%s",flags&top_flag? "T":"-",
                                                          flags&bottom_flag? "B":"-",
                                                          flags&left_flag? "L":"-",
                                                          flags&right_flag? "R":"-"),0,48);
  */
      }
  }

};


shell*
g_shell;


void
main_loop() noexcept
{
  sdl::update_control();

  g_window.do_total_reform_if_necessary();

  static uint32_t  next;

    if(g_time >= next)
    {
/*
      next = g_time+200;

      static int  x=0;
      static int  flag;

      auto&  v = g_shell->get_current()->get_view();

      auto  dir = v.get_direction();

      auto  tr = (dir == subiso::direction::front)? subiso::space_view::transform_to_front
                :(dir == subiso::direction::left)?  subiso::space_view::transform_to_left
                :(dir == subiso::direction::back)?  subiso::space_view::transform_to_back
                :                                   subiso::space_view::transform_to_right;

      v.get_node(tr(x,5),5).m_box->be_null();

        if(!flag)
        {
            if(++x >= 10)
            {
              flag = 1;
            }
        }

      else
        {
            if(!--x)
            {
              flag = 0;
            }
        }


      v.get_node(tr(x,5),5).m_box->be_earth();

      g_shell->request_redraw();
*/
    }


    if(g_modified_input.test_p() && g_input.test_p())
    {
      g_shell->request_redraw();
    }

  else
    if(g_point_buffer.size())
    {
        for(auto&  pt: g_point_buffer)
        {
          g_window.process_user_input(pt);
        }
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
                  "正方形の頂面と側面を等尺で、\n"
                  "二次元にレンダリングするプログラム\n"
                  "\n"
                  "適切な繋ぎ目を持つ画像に、自動で切り替わる\n"
                  "\n"
                  "マウスの左ボタンを押すと、ボックスを置く\n"
                  "　　　　右ボタンを押すと、ボックスを消す\n"
                  "左下のボタンで視点を回転させる\n"
                  "</pre>");

  show_github_link();
#endif


  g_space.resize(12,12,8);

    for(int  y = 0;  y < g_space.get_y_length();  ++y){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,y,0).m_kind = subiso::box::kind::earth;
    }}


    for(int  z = 0;  z < g_space.get_z_length()-1;  ++z){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,g_space.get_y_length()-1,z).m_kind = subiso::box::kind::earth;
    }}


    for(int  z = 0;  z < g_space.get_z_length()-1;  ++z){
    for(int  y = 0;  y < 3;  ++y){
      g_space.get_box(4,y,z).m_kind = subiso::box::kind::earth;
    }}


    for(int  z = 0;  z < g_space.get_z_length()-1;  ++z){
    for(int  x = 0;  x < 5;  ++x){
      g_space.get_box(x,3,z).m_kind = subiso::box::kind::earth;
    }}


  auto  l_button = new button(new iconshow({&icons::left }),[](button_event  evt){if(evt.is_press()){evt->get_userdata<shell>()->revolve_to_left() ;}});
  auto  r_button = new button(new iconshow({&icons::right}),[](button_event  evt){if(evt.is_press()){evt->get_userdata<shell>()->revolve_to_right();}});

  auto  buttons = make_row({l_button,r_button});

  g_shell = new shell(g_space);

//  g_shell->get_current()->get_view().get_node(6,6,5).m_box->set_water_source_flag();

  set_userdata({l_button,r_button},g_shell);

  g_window.set_root_widget(make_column({g_shell,buttons}));

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




