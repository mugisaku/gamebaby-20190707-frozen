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


subiso::renderer  g_front_renderer;
subiso::renderer  g_back_renderer;
subiso::renderer  g_left_renderer;
subiso::renderer  g_right_renderer;


subiso::renderer*  g_current_renderer=&g_front_renderer;


subiso::plane*
g_last_plane;


void
process_input() noexcept
{
  static bool  lock;

    for(auto&  pt: g_point_buffer)
    {
      int  x = pt.x/subiso::g_plane_size;
      int  y = pt.y/subiso::g_plane_size;


      auto&  view = g_current_renderer->get_view();

      auto  el = g_current_renderer->get_refstack(x,y).get_current();

        if(g_last_plane != el.get_plane())
        {
          lock = false;
        }


      g_last_plane = el.get_plane();

      auto&  box = *el.get_node()->m_box;

        if(!lock)
        {
          auto&  nd = *el.get_node();

            if(g_input.test_mouse_right())
            {
              box.m_kind = subiso::box::kind::null;

              lock = true;
            }

          else
            if(g_input.test_mouse_left())
            {
                if(g_last_plane->is_top() && nd.m_up_node)
                {
                  nd.m_up_node->m_box->m_kind = subiso::box::kind::earth;
                }

              else
                if(nd.m_front_node)
                {
                  nd.m_front_node->m_box->m_kind = subiso::box::kind::earth;
                }


              lock = true;
            }
        }
    }
}


void
update_screen() noexcept
{
  g_current_renderer->render(g_screen_canvas);

    if(g_last_plane)
    {
      string_form  sf;

      auto  i = g_last_plane->m_box->m_index;

//      auto  flags = g_last_box->m_top_plane.get_flags();

      static character_color  chcolor(colors::black,colors::white);

      g_screen_canvas.draw_string(chcolor,sf("X:%3d",i.x),0, 0);
      g_screen_canvas.draw_string(chcolor,sf("Y:%3d",i.y),0,16);
      g_screen_canvas.draw_string(chcolor,sf("Z:%3d",i.z),0,32);
/*
      g_screen_canvas.draw_string(chcolor,sf("%s%s%s%s",flags&top_flag? "T":"-",
                                                        flags&bottom_flag? "B":"-",
                                                        flags&left_flag? "L":"-",
                                                        flags&right_flag? "R":"-"),0,48);
*/
    }


  sdl::update_screen(g_screen_canvas);
}


void
main_loop() noexcept
{
  sdl::update_control();

    if(g_modified_input.test_p() && g_input.test_p())
    {
//      g_space.update_box_all();

      g_current_renderer = (g_current_renderer == &g_front_renderer)? &g_left_renderer
                          :(g_current_renderer == &g_left_renderer)?  &g_back_renderer
                          :(g_current_renderer == &g_back_renderer)?  &g_right_renderer
                          :&g_front_renderer;

      update_screen();
    }

  else
    if(g_point_buffer.size())
    {
      process_input();

      update_screen();
    }

  else
    if(g_needed_to_redraw)
    {
      update_screen();
    }
}


}




int
main(int  argc, char**  argv)
{
#ifdef __EMSCRIPTEN__
  set_description("<pre>"
                  "三次元空間で、頂面と前面だけを持つボックスを、\n"
                  "ふたつの面を同じ比率で二次元にレンダリングするプログラム\n"
                  "\n"
                  "適切な繋ぎ目を持つ画像に、自動で切り替わる\n"
                  "\n"
                  "マウスの左ボタンを押すと、頂面ならその上位置に、\n"
                  "                          前面ならその前位置に、ボックスを置く\n"
                  "　　　　右ボタンを押すと、ボックスを消す\n"
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
    for(int  y = 0;  y < 4;  ++y){
      g_space.get_box(4,y,z).m_kind = subiso::box::kind::earth;
    }}


    for(int  z = 0;  z < g_space.get_z_length()-1;  ++z){
    for(int  x = 0;  x < 5;  ++x){
      g_space.get_box(x,4,z).m_kind = subiso::box::kind::earth;
    }}


  auto&  box = g_space.get_box(6,g_space.get_y_length()-2,4);

  g_front_renderer.assign(subiso::space_view(g_space));
  g_left_renderer.assign(g_front_renderer.get_view().make_revolved());
  g_back_renderer.assign(g_left_renderer.get_view().make_revolved());
  g_right_renderer.assign(g_back_renderer.get_view().make_revolved());

  box.m_kind = subiso::box::kind::water;

  sdl::init(g_front_renderer.get_image_width(),g_front_renderer.get_image_height());

  g_screen_canvas = sdl::make_screen_canvas();

  update_screen();

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




