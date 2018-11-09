#include"libsubiso/subiso.hpp"




namespace subiso{




void
renderer::
clear() noexcept
{
  delete[] m_table          ;
           m_table = nullptr;
}


void
renderer::
update() noexcept
{
  int  xl = m_view.get_x_length();
  int  yl = m_view.get_y_length();
  int  zl = m_view.get_z_length();

  delete[] m_table                                        ;
           m_table = new plane_reference_stack[xl*(yl+zl)];


  std::vector<plane_reference_stack::element>  els;

    for(int  y = 0;  y < yl;  ++y){
    for(int  x = 0;  x < xl;  ++x){
      auto&  refstack = get_refstack(x,y);

      els.clear();

      auto  nd = &m_view.get_node(x,yl-1-y,zl-1);

        while(nd)
        {
          auto  pl = nd->m_top_plane;

          els.emplace_back(nd,pl);

          nd = nd->m_back_node;

            if(!nd)
            {
              break;
            }


          pl = nd->m_front_plane;

          els.emplace_back(nd,pl);

          nd = nd->m_down_node;

            if(!nd)
            {
              break;
            }
        }


      refstack.assign(els.data(),els.size());
    }}


    for(int  z = 0;  z < zl;  ++z){
    for(int  x = 0;  x < xl;  ++x){
      auto&  refstack = get_refstack(x,yl+z);

      els.clear();

      auto  nd = &m_view.get_node(x,0,zl-1-z);

        while(nd)
        {
          auto  pl = nd->m_front_plane;

          els.emplace_back(nd,pl);

          nd = nd->m_down_node;

            if(!nd)
            {
              break;
            }


          pl = nd->m_top_plane;

          els.emplace_back(nd,pl);

          nd = nd->m_back_node;

            if(!nd)
            {
              break;
            }
        }


      refstack.assign(els.data(),els.size());
    }}
}


renderer&
renderer::
assign(space_view&&  view) noexcept
{
  m_view = std::move(view);

  update();

  return *this;
}


plane_reference_stack&
renderer::
get_refstack(int  x, int  y) noexcept
{
  return m_table[(m_view.get_x_length()*y)+x];
}


const plane_reference_stack&
renderer::
get_refstack(int  x, int  y) const noexcept
{
  return m_table[(m_view.get_x_length()*y)+x];
}


void
renderer::
render(const gbstd::canvas&  cv) noexcept
{
  int  w = m_view.get_x_length()                      ;
  int  h = m_view.get_y_length()+m_view.get_z_length();

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  refstack = get_refstack(x,y);

      gbstd::canvas  cocv(cv,g_plane_size*x,g_plane_size*y,g_plane_size,g_plane_size);

      refstack.render(cocv);
    }}
}




}




