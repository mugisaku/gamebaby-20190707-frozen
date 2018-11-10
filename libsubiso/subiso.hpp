#ifndef subiso_hpp_is_included
#define subiso_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"



namespace subiso{


constexpr int  g_plane_size = 24;

constexpr int     top_flag = 1;
constexpr int  bottom_flag = 2;
constexpr int    left_flag = 4;
constexpr int   right_flag = 8;


struct
point3d
{
  int  x;
  int  y;
  int  z;

  constexpr point3d(int  x_=0, int  y_=0, int  z_=0) noexcept: x(x_), y(y_), z(z_){}

  point3d&  assign(int  x_, int  y_, int  z_) noexcept
  {
    x = x_;
    y = y_;
    z = z_;

    return *this;
  }

  void  print() const noexcept{printf("{%3d,%3d,%3d}",x,y,z);}

};


class space;
class box;
class plane;




struct
plane
{
  box*  m_box=nullptr;

  enum class kind{
     top,
    side,
  } m_kind;


  void  reset(box&  box, kind  k, plane*  up=nullptr, plane*  down=nullptr) noexcept;

  bool  is_top()   const noexcept{return m_kind == kind::top;}
  bool  is_side() const noexcept{return m_kind == kind::side;}

  void  render(int  flags, const gbstd::canvas&  cv) const noexcept;

};


struct
box
{
  space*  m_space=nullptr;

  point3d  m_index;

  enum class kind{
    null,
    earth,
    water,
  } m_kind=kind::null;

  plane  m_top_plane;
  plane  m_back_plane;
  plane  m_front_plane;
  plane  m_left_plane;
  plane  m_right_plane;

  int  m_distance=0;

  operator bool() const noexcept{return m_kind != kind::null;}

  space*  get_space(          ) const noexcept{return m_space     ;}
  void    set_space(space*  sp)       noexcept{       m_space = sp;}

  const point3d&  get_index(           ) const noexcept{return m_index     ;}
  void            set_index(point3d  pt)       noexcept{       m_index = pt;}

  int   get_distance(      ) const noexcept{return m_distance    ;}
  void  set_distance(int  v)       noexcept{       m_distance = v;}

  bool  is_null() const noexcept{return m_kind == kind::null;}
  bool  is_earth() const noexcept{return m_kind == kind::earth;}
  bool  is_water() const noexcept{return m_kind == kind::water;}

};




class
space
{
  std::vector<box>  m_boxes;

  int  m_x_length=0;
  int  m_y_length=0;
  int  m_z_length=0;

public:
  int  get_x_length() const noexcept{return m_x_length;}
  int  get_y_length() const noexcept{return m_y_length;}
  int  get_z_length() const noexcept{return m_z_length;}

  void  reset_distance_all() noexcept;

        box&  get_box(int  x, int  y, int  z)       noexcept{return m_boxes[(m_x_length*m_y_length*z)+(m_x_length*y)+x];}
  const box&  get_box(int  x, int  y, int  z) const noexcept{return m_boxes[(m_x_length*m_y_length*z)+(m_x_length*y)+x];}

  box*  get_box_pointer(int  x, int  y, int  z) noexcept;

  void  resize(int  xl, int  yl, int  zl) noexcept;

};




class
space_view
{
public:
  struct node{
    point3d  m_index;

    box*  m_box;

    plane*    m_top_plane;
    plane*  m_front_plane;
    plane*   m_back_plane;
    plane*   m_left_plane;
    plane*  m_right_plane;

    node*  m_up_node   =nullptr;
    node*  m_down_node =nullptr;

    node*  m_left_node =nullptr;
    node*  m_right_node=nullptr;
    node*  m_front_node=nullptr;
    node*  m_back_node =nullptr;

    node*  m_up_left_node =nullptr;
    node*  m_up_right_node=nullptr;
    node*  m_up_back_node =nullptr;
    node*  m_up_front_node =nullptr;

    node*  m_down_left_node =nullptr;
    node*  m_down_right_node=nullptr;
    node*  m_down_back_node =nullptr;
    node*  m_down_front_node=nullptr;

    node*  m_front_left_node =nullptr;
    node*  m_front_right_node=nullptr;
    node*  m_back_left_node  =nullptr;
    node*  m_back_right_node =nullptr;

  };

private:
  space*  m_space=nullptr;

  int  m_x_length=0;
  int  m_y_length=0;
  int  m_z_length=0;

  node*  m_nodes=nullptr;

  void  resize(int  xl, int  yl, int  zl) noexcept;

public:
  space_view() noexcept{}
  space_view(const space_view&   rhs) noexcept{assign(rhs);}
  space_view(      space_view&&  rhs) noexcept{assign(std::move(rhs));}
  space_view(space&  sp) noexcept{assign(sp);}
 ~space_view() noexcept{clear();}

  space_view&  operator=(const space_view&   rhs) noexcept{return assign(rhs);}
  space_view&  operator=(      space_view&&  rhs) noexcept{return assign(std::move(rhs));}

  space_view&  assign(const space_view&   rhs) noexcept;
  space_view&  assign(      space_view&&  rhs) noexcept;

  space_view&  operator=(space&  sp) noexcept{return assign(sp);}

  space_view&  assign(space&  sp) noexcept;

  void  clear() noexcept;

  int  get_x_length() const noexcept{return m_x_length;}
  int  get_y_length() const noexcept{return m_y_length;}
  int  get_z_length() const noexcept{return m_z_length;}

        space_view::node&  get_node(int  x, int  y, int  z)       noexcept;
  const space_view::node&  get_node(int  x, int  y, int  z) const noexcept;

  space_view::node*  get_node_pointer(int  x, int  y, int  z) noexcept;

  space_view  make_revolved() const noexcept;

};


class
plane_reference_stack
{
public:
  class element{
    space_view::node*  m_node=nullptr;
    plane*            m_plane=nullptr;

  public:
    constexpr element() noexcept{}
    constexpr element(space_view::node*  nd, plane*  pl) noexcept: m_node(nd), m_plane(pl){}

    space_view::node*  get_node() const noexcept{return m_node;}
    plane*            get_plane() const noexcept{return m_plane;}

    int  get_flags() const noexcept;

  };

private:
  element*  m_top    =nullptr;
  element*  m_current=nullptr;

public:
  plane_reference_stack() noexcept{}
  plane_reference_stack(const plane_reference_stack&   rhs) noexcept=delete;
  plane_reference_stack(      plane_reference_stack&&  rhs) noexcept=delete;
 ~plane_reference_stack(){assign(nullptr,0);}

  plane_reference_stack&  operator=(const plane_reference_stack&   rhs) noexcept=delete;
  plane_reference_stack&  operator=(      plane_reference_stack&&  rhs) noexcept=delete;

  element&  get_from_top(int  i) const noexcept{return m_top[i];}

  void  assign(const element*  els, int  n) noexcept;

  void  seek() noexcept;

  element&      get_top() const noexcept{return *m_top;}
  element&  get_current() const noexcept{return *m_current;}

  void  render(const gbstd::canvas&  cv) noexcept;

};


class
renderer
{
  space_view  m_view;

  plane_reference_stack*  m_table=nullptr;

  void  update() noexcept;

public:
  ~renderer(){clear();}

  void  clear() noexcept;

  const space_view&  get_view() const noexcept{return m_view;}

  int  get_image_width()  const noexcept{return g_plane_size*(m_view.get_x_length()                      );}
  int  get_image_height() const noexcept{return g_plane_size*(m_view.get_y_length()+m_view.get_z_length());}

  renderer&  assign(space_view&&  view) noexcept;

        plane_reference_stack&  get_refstack(int  x, int  y)       noexcept;
  const plane_reference_stack&  get_refstack(int  x, int  y) const noexcept;

  void  render(const gbstd::canvas&  cv) noexcept;

};


}



#endif




