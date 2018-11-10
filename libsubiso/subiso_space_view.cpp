#include"libsubiso/subiso.hpp"




namespace subiso{




space_view::node&
space_view::get_node(int  x, int  y, int  z) noexcept
{
  return m_nodes[(m_x_length*m_y_length*z)+(m_x_length*y)+x];
}


const space_view::node&
space_view::get_node(int  x, int  y, int  z) const noexcept
{
  return m_nodes[(m_x_length*m_y_length*z)+(m_x_length*y)+x];
}


space_view::node*
space_view::
get_node_pointer(int  x, int  y, int  z) noexcept
{
    if((x >= 0) && (x < m_x_length) &&
       (y >= 0) && (y < m_y_length) &&
       (z >= 0) && (z < m_z_length))
    {
      return &get_node(x,y,z);
    }


  return nullptr;
}


void
space_view::
clear() noexcept
{
  m_space = nullptr;

  m_x_length = 0;
  m_y_length = 0;
  m_z_length = 0;

  delete[] m_nodes          ;
           m_nodes = nullptr;
}


void
space_view::
resize(int  xl, int  yl, int  zl) noexcept
{
  m_x_length = xl;
  m_y_length = yl;
  m_z_length = zl;

  delete[] m_nodes                                             ;
           m_nodes = new node[m_x_length*m_y_length*m_z_length];

    for(int  z = 0;  z < m_z_length;  ++z){
    for(int  y = 0;  y < m_y_length;  ++y){
    for(int  x = 0;  x < m_x_length;  ++x){
      auto&  node = get_node(x,y,z);

      node.m_index = point3d(x,y,z);

      node.m_front_node = get_node_pointer(x  ,y-1,z  );
      node.m_back_node  = get_node_pointer(x  ,y+1,z  );
      node.m_left_node  = get_node_pointer(x-1,y  ,z  );
      node.m_right_node = get_node_pointer(x+1,y  ,z  );
      node.m_down_node  = get_node_pointer(x  ,y  ,z-1);
      node.m_up_node    = get_node_pointer(x  ,y  ,z+1);

      node.m_up_left_node  = get_node_pointer(x-1,y  ,z+1);
      node.m_up_right_node = get_node_pointer(x+1,y  ,z+1);
      node.m_up_back_node  = get_node_pointer(x  ,y+1,z+1);
      node.m_up_front_node = get_node_pointer(x  ,y-1,z+1);

      node.m_down_left_node  = get_node_pointer(x-1,y  ,z-1);
      node.m_down_right_node = get_node_pointer(x+1,y  ,z-1);
      node.m_down_back_node  = get_node_pointer(x  ,y+1,z-1);
      node.m_down_front_node = get_node_pointer(x  ,y-1,z-1);

      node.m_back_left_node   = get_node_pointer(x-1,y+1,z);
      node.m_back_right_node  = get_node_pointer(x+1,y+1,z);
      node.m_front_left_node  = get_node_pointer(x-1,y-1,z);
      node.m_front_right_node = get_node_pointer(x+1,y-1,z);
    }}}
}




space_view&
space_view::
assign(const space_view&  rhs) noexcept
{
    if(&rhs != this)
    {
      resize(rhs.m_x_length,rhs.m_y_length,rhs.m_z_length);

      size_t  n = rhs.m_x_length*rhs.m_y_length*rhs.m_z_length;

      m_space = rhs.m_space;

      auto  src = rhs.m_nodes;
      auto  dst =     m_nodes;

        while(n--)
        {
          dst->m_box = src->m_box;

          dst->m_top_plane   = src->m_top_plane;
          dst->m_front_plane = src->m_front_plane;
          dst->m_back_plane  = src->m_back_plane;
          dst->m_left_plane  = src->m_left_plane;
          dst->m_right_plane = src->m_right_plane;

          ++src;
          ++dst;
        }
    }


  return *this;
}


space_view&
space_view::
assign(space_view&&  rhs) noexcept
{
    if(&rhs != this)
    {
      clear();

      std::swap(m_space   ,rhs.m_space);
      std::swap(m_nodes   ,rhs.m_nodes);
      std::swap(m_x_length,rhs.m_x_length);
      std::swap(m_y_length,rhs.m_y_length);
      std::swap(m_z_length,rhs.m_z_length);
    }


  return *this;
}


space_view&
space_view::
assign(space&  sp) noexcept
{
  resize(sp.get_x_length(),sp.get_y_length(),sp.get_z_length());

  m_space = &sp;

    for(int  z = 0;  z < m_z_length;  ++z){
    for(int  y = 0;  y < m_y_length;  ++y){
    for(int  x = 0;  x < m_x_length;  ++x){
      auto&   box = sp.get_box(x,y,z);
      auto&  node =   get_node(x,y,z);

      node.m_box = &box;

      node.m_top_plane   = &box.m_top_plane;
      node.m_back_plane  = &box.m_back_plane;
      node.m_front_plane = &box.m_front_plane;
      node.m_left_plane  = &box.m_left_plane;
      node.m_right_plane = &box.m_right_plane;
    }}}


  return *this;
}


space_view
space_view::
make_revolved() const noexcept
{
  space_view  sv;

  sv.resize(m_y_length,m_x_length,m_z_length);

  sv.m_space = m_space;

    for(int  z = 0;  z < m_z_length;  ++z){
    for(int  y = 0;  y < m_y_length;  ++y){
    for(int  x = 0;  x < m_x_length;  ++x){
      auto&  src =    get_node(x,y,z);
      auto&  dst = sv.get_node(m_y_length-1-y,x,z);

      dst.m_box = src.m_box;

      dst.m_top_plane   = src.m_top_plane;
      dst.m_right_plane = src.m_front_plane;
      dst.m_front_plane = src.m_left_plane;
      dst.m_left_plane  = src.m_back_plane;
      dst.m_back_plane  = src.m_right_plane;
    }}}


  return std::move(sv);
}




}




