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
g_binary[] =
{
//#include"noName.txt"
};


image
g_graphic_image(g_binary);


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


void
plane::
render(const canvas&  cv) const noexcept
{
  auto&  box = *m_box;

  int  z = box.m_index.z;

    if(m_kind == kind::top)
    {
      auto  color = box.m_focus? colors::red
                   :      gbstd::color(0,z+1,z+1);

      cv.fill_rectangle(color,0,0,g_plane_size,g_plane_size);

        if(!is_valid(box.m_left_box )){cv.draw_vline(colors::black,             0,0,g_plane_size);}
        if(!is_valid(box.m_right_box)){cv.draw_vline(colors::black,g_plane_size-1,0,g_plane_size);}
        if(!is_valid(box.m_back_box )){cv.draw_hline(colors::white,0,             0,g_plane_size);}
        if(!is_valid(box.m_front_box)){cv.draw_hline(colors::black,0,g_plane_size-1,g_plane_size);}
    }

  else
    {
      auto  color = box.m_focus? colors::red
                   :      gbstd::color(7,z,z);

      cv.fill_rectangle(color,0,0,g_plane_size,g_plane_size);

        if(!is_valid(box.m_left_box )){cv.draw_vline(colors::black,             0,0,g_plane_size);}
        if(!is_valid(box.m_right_box)){cv.draw_vline(colors::black,g_plane_size-1,0,g_plane_size);}

        if(is_valid(box.m_down_box             ) &&
           is_valid(box.m_down_box->m_front_box))
        {
          cv.draw_hline(colors::black,0,g_plane_size-1,g_plane_size);
        }
    }
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

          if(x         ){box.m_left_box  = &get_box(x-1,y  ,z  );}
          if(y         ){box.m_front_box = &get_box(x  ,y-1,z  );}
          if(z         ){box.m_down_box  = &get_box(x  ,y  ,z-1);}
          if(x < (xl-1)){box.m_right_box = &get_box(x+1,y  ,z  );}
          if(y < (yl-1)){box.m_back_box  = &get_box(x  ,y+1,z  );}
          if(z < (zl-1)){box.m_up_box    = &get_box(x  ,y  ,z+1);}


        box.m_index = point3d{x,y,z};

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
                if(cur->is_top() && box.m_up_box)
                {
                  box.m_up_box->m_valid = true;
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

      static character_color  chcolor(colors::black,colors::white);

      g_screen_canvas.draw_string(chcolor,sf("X:%3d",i.x),0, 0);
      g_screen_canvas.draw_string(chcolor,sf("Y:%3d",i.y),0,16);
      g_screen_canvas.draw_string(chcolor,sf("Z:%3d",i.z),0,32);
    }


  sdl::update_screen(g_screen_canvas);
}


void
main_loop() noexcept
{
  sdl::update_control();

  bool  procssed = false;

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
                  "マウスの左ボタンを押すと、ボックスを置く\n"
                  "　　　　右ボタンを押すと、ボックスを消す\n"
                  "</pre>");

  show_github_link();
#endif


  g_space.resize(12,12,6);

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




