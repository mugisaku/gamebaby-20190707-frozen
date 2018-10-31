#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/value.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/window.hpp"
#include"libgbstd/windows/component.hpp"
#include"libge/ge.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas
g_screen_canvas;


const uint8_t
g_green_binary[] =
{
#include"green.txt"
};


const uint8_t
g_wall_binary[] =
{
#include"wall.txt"
};


image
g_green_image(g_green_binary);


image
g_wall_image(g_wall_binary);


constexpr int  g_plane_size = 24;


struct
point3d
{
  int  x;
  int  y;
  int  z;

};


class box;
class plane;
class node;


struct
node
{
  plane*  m_top;
  plane*  m_current;

  void  seek() noexcept;

};


struct
plane
{
  box*  m_box;

  node*  m_node;

  enum class kind{
      top,
    front,
  } m_kind;

  plane*  m_up  =nullptr;
  plane*  m_down=nullptr;

  bool  is_top()   const noexcept{return m_kind == kind::top;}
  bool  is_front() const noexcept{return m_kind == kind::front;}

  int  get_flags() const noexcept;

  void  render(const canvas&  cv) const noexcept;

};


struct
box
{
  point3d  m_index;

  bool  m_valid=true;
  bool  m_focus=false;

  plane  m_top_plane  ;
  plane  m_front_plane;

  box*  m_up_box   =nullptr;
  box*  m_down_box =nullptr;
  box*  m_left_box =nullptr;
  box*  m_right_box=nullptr;
  box*  m_front_box=nullptr;
  box*  m_back_box =nullptr;

  box*  m_left_up_box =nullptr;
  box*  m_right_up_box=nullptr;
  box*  m_back_up_box =nullptr;

  box*  m_left_front_box =nullptr;
  box*  m_right_front_box=nullptr;
  box*  m_down_front_box =nullptr;

};


bool  is_valid(const box*  ptr){return ptr? ptr->m_valid:false;}


void
node::
seek() noexcept
{
  m_current = m_top;

    while(!m_current->m_box->m_valid)
    {
      auto  down = m_current->m_down;

        if(down)
        {
          m_current = down;
        }

      else
        {
          break;
        }
    }
}


constexpr int     top_flag = 1;
constexpr int  bottom_flag = 2;
constexpr int    left_flag = 4;
constexpr int   right_flag = 8;


int
plane::
get_flags() const noexcept
{
  auto&  box = *m_box;

  int  flags = 0;

    if(m_kind == kind::top)
    {
        if(box.m_up_box)
        {
            if(!is_valid(box.m_right_box) || is_valid(box.m_right_up_box)){flags |=  right_flag;}
            if(!is_valid(box.m_left_box)  || is_valid(box.m_left_up_box) ){flags |=   left_flag;}
            if(!is_valid(box.m_back_box)  || is_valid(box.m_back_up_box) ){flags |=    top_flag;}
            if(!is_valid(box.m_front_box)                                ){flags |= bottom_flag;}
        }
    }

  else
    {
        if(!is_valid(box.m_right_box) || is_valid(box.m_right_front_box)){flags |=  right_flag;}
        if(!is_valid(box.m_left_box)  || is_valid(box.m_left_front_box) ){flags |=   left_flag;}
        if(!is_valid(box.m_down_box)  || is_valid(box.m_down_front_box) ){flags |= bottom_flag;}
        if(!is_valid(box.m_up_box)                                      ){flags |=    top_flag;}
    }


  return flags;
}


void
plane::
render(const canvas&  cv) const noexcept
{
  gbstd::point  pt(1,1);

    switch(get_flags())
    {
  case(top_flag|left_flag    ): pt = {0,0};break;
  case(top_flag              ): pt = {1,0};break;
  case(top_flag|right_flag   ): pt = {2,0};break;
  case(left_flag             ): pt = {0,1};break;
  case(right_flag            ): pt = {2,1};break;
  case(left_flag| bottom_flag): pt = {0,2};break;
  case(           bottom_flag): pt = {1,2};break;
  case(right_flag|bottom_flag): pt = {2,2};break;

  case( top_flag|bottom_flag): pt = {3,0};break;
  case(left_flag| right_flag): pt = {4,0};break;

  case(top_flag| left_flag|bottom_flag): pt = {3,1};break;
  case(top_flag|right_flag|bottom_flag): pt = {4,1};break;

  case(left_flag|right_flag|bottom_flag): pt = {3,2};break;
  case(left_flag|right_flag|   top_flag): pt = {4,2};break;

  case(top_flag|left_flag|right_flag|bottom_flag): pt = {0,3};break;
    }


  gbstd::canvas  bgcv;

    if(m_kind == kind::top)
    {
      bgcv = canvas(g_green_image,g_plane_size*pt.x,g_plane_size*pt.y,g_plane_size,g_plane_size);
    }

  else
    {
      bgcv = canvas(g_wall_image,g_plane_size*pt.x,g_plane_size*pt.y,g_plane_size,g_plane_size);
    }


  cv.draw_canvas(bgcv,0,0);
}


struct
space
{
  std::vector<box>  m_boxes;

  int  m_x_length=0;
  int  m_y_length=0;
  int  m_z_length=0;

  int  get_image_width()  const noexcept{return g_plane_size*m_x_length;}
  int  get_image_height() const noexcept{return g_plane_size*(m_y_length+m_z_length);}

  std::vector<node>  m_map;

  int  get_x_length() const noexcept{return m_x_length;}
  int  get_y_length() const noexcept{return m_y_length;}
  int  get_z_length() const noexcept{return m_z_length;}

  box&  get_box(int  x, int  y, int  z) noexcept{return m_boxes[(m_x_length*m_y_length*z)+(m_x_length*y)+x];}

  box*  get_box_pointer(int  x, int  y, int  z) noexcept
  {
      if((x >= 0) && (x < m_x_length) &&
         (y >= 0) && (y < m_y_length) &&
         (z >= 0) && (z < m_z_length))
      {
        return &get_box(x,y,z);
      }


    return nullptr;
  }

  static plane*  get_front(box*  ptr) noexcept{return ptr? &ptr->m_front_plane:nullptr;}
  static plane*  get_top(  box*  ptr) noexcept{return ptr? &ptr->m_top_plane  :nullptr;}

  plane*  get_front(int  x, int  y, int  z) noexcept{return get_front(get_box_pointer(x,y,z));}
  plane*  get_top(  int  x, int  y, int  z) noexcept{return get_top(  get_box_pointer(x,y,z));}

  void  resize(int  xl, int  yl, int  zl) noexcept
  {
    m_boxes.resize(xl*(yl*zl));
    m_map.resize(  xl*(yl+zl));

    m_x_length = xl;
    m_y_length = yl;
    m_z_length = zl;

      for(int  z = 0;  z < zl;  ++z){
      for(int  y = 0;  y < yl;  ++y){
      for(int  x = 0;  x < xl;  ++x){
        auto&  box = get_box(x,y,z);

        bool   left_exists = x         ;
        bool  right_exists = x < (xl-1);
        bool  front_exists = y         ;
        bool   back_exists = y < (yl-1);
        bool     up_exists = z         ;
        bool   down_exists = z < (zl-1);

          if(left_exists ){box.m_left_box  = &get_box(x-1,y  ,z  );}
          if(back_exists ){box.m_back_box  = &get_box(x  ,y+1,z  );}
          if(up_exists   ){box.m_down_box  = &get_box(x  ,y  ,z-1);}
          if(right_exists){box.m_right_box = &get_box(x+1,y  ,z  );}
          if(front_exists){box.m_front_box = &get_box(x  ,y-1,z  );}
          if(down_exists ){box.m_up_box    = &get_box(x  ,y  ,z+1);}
          if( left_exists && up_exists){box.m_left_up_box  = &get_box(x-1,y  ,z+1);}
          if(right_exists && up_exists){box.m_right_up_box = &get_box(x+1,y  ,z+1);}
          if(back_exists  && up_exists){box.m_back_up_box  = &get_box(x  ,y+1,z+1);}
          if( left_exists && front_exists){box.m_left_front_box  = &get_box(x-1,y-1,z  );}
          if(right_exists && front_exists){box.m_right_front_box = &get_box(x+1,y-1,z  );}
          if(down_exists  && front_exists){box.m_down_front_box  = &get_box(x  ,y-1,z-1);}


        box.m_index = point3d{x,y,z};

        box.m_valid = false;

        box.m_front_plane.m_box  = &box;
        box.m_front_plane.m_kind = plane::kind::front;
        box.m_front_plane.m_up   = get_top(x,y-1,z  );
        box.m_front_plane.m_down = get_top(x,y  ,z-1);

        box.m_top_plane.m_box  = &box;
        box.m_top_plane.m_kind = plane::kind::top;
        box.m_top_plane.m_up   = get_front(x,y  ,z+1);
        box.m_top_plane.m_down = get_front(x,y+1,z  );
      }}}


    auto  p = m_map.data();

      for(int  y = 0;  y < yl;  ++y){
      for(int  x = 0;  x < xl;  ++x){
        auto&  nd = *p++;

                       nd.m_top = &get_box(x,m_y_length-1-y,m_z_length-1).m_top_plane;
        nd.m_current = nd.m_top                                                      ;


        auto  next = nd.m_current;

          while(next)
          {
            next->m_node = &nd;

            next = next->m_down;
          }
      }}


      for(int  z = 0;  z < zl;  ++z){
      for(int  x = 0;  x < xl;  ++x){
        auto&  nd = *p++;

                       nd.m_top = &get_box(x,0,m_z_length-1-z).m_front_plane;
        nd.m_current = nd.m_top                                             ;


        auto  next = nd.m_current;

          while(next)
          {
            next->m_node = &nd;

            next = next->m_down;
          }
      }}
  }

  node&  get_node(int  x, int  y) noexcept{return m_map[(m_x_length*y)+x];}

  void  update() noexcept
  {
    int  h = m_y_length+m_z_length;

      for(int  y = 0;  y <          h;  ++y){
      for(int  x = 0;  x < m_x_length;  ++x){
        auto&  nd = get_node(x,y);

        nd.seek();
      }}
  }

  void  render(const canvas&  cv) noexcept
  {
    int  h = m_y_length+m_z_length;

      for(int  y = 0;  y <          h;  ++y){
      for(int  x = 0;  x < m_x_length;  ++x){
        canvas  cocv(cv,g_plane_size*x,g_plane_size*y,g_plane_size,g_plane_size);

        auto&  nd = get_node(x,y);

        nd.seek();

        auto  cur = nd.m_current;

          if(cur)
          {
            cur->render(cocv);
          }
      }}
  }

};


space
g_space;


box*
g_last_box;


void
process_input() noexcept
{
  static node*  last_node;

  static bool  lock;

    for(auto&  pt: g_point_buffer)
    {
      int  x = pt.x/g_plane_size;
      int  y = pt.y/g_plane_size;

        if(g_last_box)
        {
          g_last_box->m_focus = false;
        }


      auto&  nd = g_space.get_node(x,y);

        if(last_node != &nd)
        {
          lock = false;
        }


      auto  cur = nd.m_current;

      auto&  box = *cur->m_box;


      box.m_focus = true;

        if(!lock)
        {
            if(g_input.test_mouse_right() && box.m_index.z && (box.m_index.y < (g_space.m_y_length-1)))
            {
              box.m_valid = false;

              lock = true;
            }

          else
            if(g_input.test_mouse_left())
            {
                if(cur->is_top())
                {
                    if(box.m_up_box && box.m_up_box->m_up_box)
                    {
                      box.m_up_box->m_valid = true;
                    }
                }

              else
                if(box.m_front_box)
                {
                  box.m_front_box->m_valid = true;
                }


              lock = true;
            }
        }


      g_last_box  = &box;
        last_node = &nd;
    }


}


void
update_screen() noexcept
{
  g_space.update();

  g_space.render(g_screen_canvas);

    if(g_last_box)
    {
      string_form  sf;

      auto  i = g_last_box->m_index;

      auto  flags = g_last_box->m_top_plane.get_flags();

      static character_color  chcolor(colors::black,colors::white);

      g_screen_canvas.draw_string(chcolor,sf("X:%3d",i.x),0, 0);
      g_screen_canvas.draw_string(chcolor,sf("Y:%3d",i.y),0,16);
      g_screen_canvas.draw_string(chcolor,sf("Z:%3d",i.z),0,32);
      g_screen_canvas.draw_string(chcolor,sf("%s%s%s%s",flags&top_flag? "T":"-",
                                                        flags&bottom_flag? "B":"-",
                                                        flags&left_flag? "L":"-",
                                                        flags&right_flag? "R":"-"),0,48);
    }


  sdl::update_screen(g_screen_canvas);
}


void
main_loop() noexcept
{
  sdl::update_control();

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


  g_space.resize(12,12,6);

    for(int  y = 0;  y < g_space.get_y_length();  ++y){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,y,0).m_valid = true;
    }}


    for(int  z = 0;  z < g_space.get_z_length();  ++z){
    for(int  x = 0;  x < g_space.get_x_length();  ++x){
      g_space.get_box(x,g_space.get_y_length()-1,z).m_valid = true;
    }}


  sdl::init(g_space.get_image_width(),g_space.get_image_height());

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




