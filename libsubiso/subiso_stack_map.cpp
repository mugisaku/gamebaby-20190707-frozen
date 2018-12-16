#include"libsubiso/subiso.hpp"




namespace subiso{




void
stack_map::
clear() noexcept
{
  delete[] m_table          ;
           m_table = nullptr;

  m_width  = 0;
  m_height = 0;

  m_image_width  = 0;
  m_image_height = 0;

  m_upper_image_height = 0;
  m_lower_image_height = 0;
}


stack_map&
stack_map::
assign(space_view  sv) noexcept
{
  int  xl = sv.get_x_length();
  int  yl = sv.get_y_length();
  int  zl = sv.get_z_length();

  m_width  = (xl   );
  m_height = (yl+zl);

  m_upper_image_height = (g_plane_size*yl);
  m_lower_image_height = (g_plane_size*zl);

  m_image_width  = (g_plane_size*m_width);
  m_image_height = (m_upper_image_height+m_lower_image_height);

  delete[] m_table                                              ;
           m_table = new plane_reference_stack[m_width*m_height];


  std::vector<plane_reference>  refs;

  m_dir = sv.get_direction();

  box_view  bv(m_dir);

    for(int  y = 0;  y < yl;  ++y){
    for(int  x = 0;  x < xl;  ++x){
      refs.clear();

      auto  box = &sv.get_box(x,yl-1-y,zl-1);

        while(box)
        {
          bv.set_box(*box);

          auto  pl = &bv.get_top_plane();

          refs.emplace_back(m_dir,pl);

          box = bv.get_back_box();

            if(!box)
            {
              break;
            }


          bv.set_box(*box);

          pl = &bv.get_front_plane();

          refs.emplace_back(m_dir,pl);

          box = bv.get_down_box();
        }


      refs.emplace_back(m_dir,nullptr);

      get_stack(x,y).assign(refs.data(),refs.size());
    }}


    for(int  z = 0;  z < zl;  ++z){
    for(int  x = 0;  x < xl;  ++x){
      refs.clear();

      auto  box = &sv.get_box(x,0,zl-1-z);

        while(box)
        {
          bv.set_box(*box);

          auto  pl = &bv.get_front_plane();

          refs.emplace_back(m_dir,pl);

          box = bv.get_down_box();

            if(!box)
            {
              break;
            }


          bv.set_box(*box);

          pl = &bv.get_top_plane();

          refs.emplace_back(m_dir,pl);

          box = bv.get_back_box();
        }


      refs.emplace_back(m_dir,nullptr);

      get_stack(x,yl+z).assign(refs.data(),refs.size());
    }}


  return *this;
}


void
stack_map::
render_line_block(int  x_quo, int  y_quo, const gbstd::canvas&  cv, int  x, int  y) noexcept
{
    for(;;)
    {
      auto&  stack = get_stack(x_quo++,y_quo);

      stack.render(m_dir,cv,x,y);

      x += g_plane_size;

        if(x >= cv.get_width())
        {
          break;
        }


        if(x_quo >= m_width)
        {
          x_quo = 0;
        }
    }
}


gbstd::point
stack_map::
transform(const point3d&  src) const noexcept
{
  int  x;
  int  y;

  int   w = get_image_width()       ;
  int  uh = get_upper_image_height();
  int  lh = get_lower_image_height();

    switch(m_dir)
    {
  case(directions::front):
      x = src.x;
      y = src.y;

      return gbstd::point(x,(uh-1-y)+(lh-1-src.z));
      break;
  case(directions::right):
      x =    src.y;
      y = uh-src.x;

      return gbstd::point(x,(uh-1-y)+(lh-1-src.z));
      break;
  case(directions::back):
      x =  w-src.x;
      y = uh-src.y;

      return gbstd::point(x,(uh-1-y)+(lh-1-src.z));
      break;
  case(directions::left):
      x = w-src.y;
      y =   src.x;

      return gbstd::point(x,(uh-1-y)+(lh-1-src.z));
      break;
    }


  return gbstd::point();
}


void
stack_map::
render(gbstd::point  view_center_pos, const gbstd::canvas&  cv) noexcept
{
  int  x_off = (view_center_pos.x-(cv.get_width() /2));
  int  y_off = (view_center_pos.y-(cv.get_height()/2));

  x_off %= m_image_width ;
  y_off %= m_image_height;

    if(x_off < 0){x_off += m_image_width ;}
    if(y_off < 0){y_off += m_image_height;}


  int  x_quo = x_off/g_plane_size;
  int  y_quo = y_off/g_plane_size;
  int  x_rem = x_off%g_plane_size;
  int  y_rem = y_off%g_plane_size;

  int  x = -x_rem;
  int  y = -y_rem;

    for(;;)
    {
      render_line_block(x_quo,y_quo++,cv,x,y);

      y += g_plane_size;

        if(y >= cv.get_height())
        {
          break;
        }


        if(y_quo >= m_height)
        {
          y_quo = 0;
        }
    }
}


void
stack_map::
print() const noexcept
{
  printf("{\n");

    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x < m_width ;  ++x){
      auto&  stack = get_stack(x,y);

      printf("%3d,",stack.get_length());
    }printf("\n");}


  printf("}\n");
}




}




