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

  m_image_width  = (g_plane_size*m_width );
  m_image_height = (g_plane_size*m_height);

  delete[] m_table                                              ;
           m_table = new plane_reference_stack[m_width*m_height];


  std::vector<plane_reference>  refs;

  auto  dir = sv.get_direction();

  box_view  bv(dir);

    for(int  y = 0;  y < yl;  ++y){
    for(int  x = 0;  x < xl;  ++x){
      refs.clear();

      auto  box = &sv.get_box(x,yl-1-y,zl-1);

        while(box)
        {
          bv.set_box(*box);

          auto  pl = &bv.get_top_plane();

          refs.emplace_back(dir,pl);

          box = bv.get_back_box();

            if(!box)
            {
              break;
            }


          bv.set_box(*box);

          pl = &bv.get_front_plane();

          refs.emplace_back(dir,pl);

          box = bv.get_down_box();
        }


      refs.emplace_back(dir,nullptr);

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

          refs.emplace_back(dir,pl);

          box = bv.get_down_box();

            if(!box)
            {
              break;
            }


          bv.set_box(*box);

          pl = &bv.get_top_plane();

          refs.emplace_back(dir,pl);

          box = bv.get_back_box();
        }


      refs.emplace_back(dir,nullptr);

      get_stack(x,yl+z).assign(refs.data(),refs.size());
    }}


  return *this;
}


void
stack_map::
render(const gbstd::canvas&  cv) noexcept
{
  int  w = m_width ;
  int  h = m_height;

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  stack = get_stack(x,y);

      gbstd::canvas  cocv(cv,g_plane_size*x,g_plane_size*y,g_plane_size,g_plane_size);

      stack.render(cocv);
    }}
}




}




