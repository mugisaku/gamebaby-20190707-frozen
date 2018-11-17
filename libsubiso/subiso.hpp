#ifndef subiso_hpp_is_included
#define subiso_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include<list>



namespace subiso{


constexpr int  g_plane_size = 24;

constexpr int     top_flag = 1;
constexpr int  bottom_flag = 2;
constexpr int    left_flag = 4;
constexpr int   right_flag = 8;


enum class
direction
{
  front,
  left,
  right,
  back,
};


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

  constexpr point3d  transform_to_left()  const noexcept{return point3d( y, x,z);}
  constexpr point3d  transform_to_back()  const noexcept{return point3d(-x,-y,z);}
  constexpr point3d  transform_to_right() const noexcept{return point3d(-y,-x,z);}

  void  print() const noexcept{printf("{%3d,%3d,%3d}",x,y,z);}

};


class space;
class box;
class plane;




struct
plane
{
  static const int  m_waterization_level;

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




constexpr int  g_water_value_max = 266;


struct
box
{
  space*  m_space=nullptr;

  point3d  m_index;

  enum class kind{
    null,
    earth,
  } m_kind=kind::null;

  plane  m_top_plane;
  plane  m_side_planes[4];

  box*  m_up_box  =nullptr;
  box*  m_down_box=nullptr;

  box*   m_upper_boxes[8];
  box*  m_middle_boxes[8];
  box*   m_lower_boxes[8];

  int  m_distance=0;

  struct flags{
    static constexpr uint32_t        active = 1;
    static constexpr uint32_t  water_source = 2;
  };


  uint32_t  m_search_flags;

  uint32_t  m_state=0;

  int  m_main_counter=0;
  int  m_water_value=0;

  operator bool() const noexcept{return m_kind != kind::null;}

  bool  test_active_flag() const noexcept{return m_state&flags::active;}
  bool  test_water_source_flag() const noexcept{return m_state&flags::water_source;}

  void  set_active_flag() noexcept{m_state |= flags::active;}
  void  set_water_source_flag() noexcept{m_state |= flags::water_source;}

  void  unset_active_flag() noexcept{m_state &= ~flags::active;}
  void  unset_water_source_flag() noexcept{m_state &= ~flags::water_source;}

  space*  get_space(          ) const noexcept{return m_space     ;}
  void    set_space(space*  sp)       noexcept{       m_space = sp;}

  const point3d&  get_index(           ) const noexcept{return m_index     ;}
  void            set_index(point3d  pt)       noexcept{       m_index = pt;}

  int   get_distance(      ) const noexcept{return m_distance    ;}
  void  set_distance(int  v)       noexcept{       m_distance = v;}

  void  be_null()  noexcept{m_kind = kind::null;}
  void  be_earth() noexcept{m_kind = kind::earth;}

  bool  is_null() const noexcept{return m_kind == kind::null;}
  bool  is_earth() const noexcept{return m_kind == kind::earth;}

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
  void  clear_search_flags() noexcept;

        box&  get_box(int  x, int  y, int  z)       noexcept{return m_boxes[(m_x_length*m_y_length*z)+(m_x_length*y)+x];}
  const box&  get_box(int  x, int  y, int  z) const noexcept{return m_boxes[(m_x_length*m_y_length*z)+(m_x_length*y)+x];}

  box&  get_box_rx( int  x, int  y, int  z) noexcept{return get_box(m_x_length-1-x,             y,z);}
  box&  get_box_ry( int  x, int  y, int  z) noexcept{return get_box(             x,m_y_length-1-y,z);}
  box&  get_box_rxy(int  x, int  y, int  z) noexcept{return get_box(m_x_length-1-x,m_y_length-1-y,z);}

  box*  get_box_pointer(int  x, int  y, int  z) noexcept;

  std::vector<box*>  search_slice(int  x, int  y, int  z) noexcept;

  void  resize(int  xl, int  yl, int  zl) noexcept;

};




class
box_view
{
  box*  m_box=nullptr;

  direction  m_dir=direction::front;

  box*  u(int  i) const noexcept{return m_box->m_upper_boxes[i];}
  box*  m(int  i) const noexcept{return m_box->m_middle_boxes[i];}
  box*  l(int  i) const noexcept{return m_box->m_lower_boxes[i];}
  plane&  pl(int  i) const noexcept{return m_box->m_side_planes[i];}

public:
  box_view() noexcept{}
  box_view(direction  dir) noexcept: m_dir(dir){}
  box_view(subiso::box&  box, direction  dir) noexcept{assign(box,dir);}

  box_view&  assign(subiso::box&  box, direction  dir) noexcept;

  bool  is_front() const noexcept{return m_dir == direction::front;}
  bool  is_left()  const noexcept{return m_dir == direction::left;}
  bool  is_right() const noexcept{return m_dir == direction::right;}
  bool  is_back()  const noexcept{return m_dir == direction::back;}

  plane&  get_top_plane() const noexcept{return m_box->m_top_plane;}

  plane&  get_front_plane() const noexcept;
  plane&   get_left_plane() const noexcept;
  plane&  get_right_plane() const noexcept;
  plane&   get_back_plane() const noexcept;

  void  set_box(subiso::box&  box)       noexcept{       m_box = &box;}
  box*  get_box(                 ) const noexcept{return m_box       ;}

  void       set_direction(direction  dir)       noexcept{       m_dir = dir;}
  direction  get_direction(              ) const noexcept{return m_dir      ;}

  box*     get_up_box() const noexcept{return m_box->m_up_box;}
  box*   get_down_box() const noexcept{return m_box->m_down_box;}

  box*    get_left_box() const noexcept;
  box*   get_right_box() const noexcept;
  box*   get_front_box() const noexcept;
  box*    get_back_box() const noexcept;

  box*   get_left_up_box()         const noexcept;
  box*   get_left_down_box()       const noexcept;
  box*   get_left_back_box()       const noexcept;
  box*   get_left_back_down_box()  const noexcept;
  box*   get_left_back_up_box()    const noexcept;
  box*   get_left_front_box()      const noexcept;
  box*   get_left_front_down_box() const noexcept;
  box*   get_left_front_up_box()   const noexcept;

  box*   get_right_up_box()         const noexcept;
  box*   get_right_down_box()       const noexcept;
  box*   get_right_back_box()       const noexcept;
  box*   get_right_back_down_box()  const noexcept;
  box*   get_right_back_up_box()    const noexcept;
  box*   get_right_front_box()      const noexcept;
  box*   get_right_front_down_box() const noexcept;
  box*   get_right_front_up_box()   const noexcept;

};


class
space_view
{
  space*  m_space=nullptr;

  int  m_x_length=0;
  int  m_y_length=0;
  int  m_z_length=0;

  direction  m_dir=direction::front;

public:
  space_view() noexcept{}
  space_view(space&  sp, direction  dir=direction::front) noexcept{assign(sp,dir);}

  space_view&  assign(space&  sp, direction  dir=direction::front) noexcept;

  bool  is_front() const noexcept{return m_dir == direction::front;}
  bool  is_left()  const noexcept{return m_dir == direction::left;}
  bool  is_right() const noexcept{return m_dir == direction::right;}
  bool  is_back()  const noexcept{return m_dir == direction::back;}

  void  update_size() noexcept;

  void    set_space(space&  sp)       noexcept{       m_space = &sp;}
  space*  get_space(          ) const noexcept{return m_space      ;}

  void       set_direction(direction  dir)       noexcept{       m_dir = dir;}
  direction  get_direction(              ) const noexcept{return m_dir      ;}

  int  get_x_length() const noexcept{return m_x_length;}
  int  get_y_length() const noexcept{return m_y_length;}
  int  get_z_length() const noexcept{return m_z_length;}
\
  box&  get_box(int  x, int  y, int  z) const noexcept;

};


class
plane_reference
{
  box*  m_left_box =nullptr;
  box*  m_right_box=nullptr;

  struct tops{
    box*  m_left_up_box =nullptr;
    box*  m_back_up_box =nullptr;
    box*  m_right_up_box=nullptr;
    box*  m_front_box   =nullptr;
    box*  m_back_box    =nullptr;
  };

  struct fronts{
    box*  m_up_box         =nullptr;
    box*  m_down_box       =nullptr;
    box*  m_front_down_box =nullptr;
    box*  m_left_front_box =nullptr;
    box*  m_right_front_box=nullptr;
  };

  union{
    tops      m_tops;
    fronts  m_fronts;
  };

  plane*  m_plane=nullptr;

  void    reset_top_link(const box_view&  bv) noexcept;
  void  reset_front_link(const box_view&  bv) noexcept;

public:
  plane_reference() noexcept{}
  plane_reference(direction  dir, plane*  pl) noexcept{assign(dir,pl);}

  plane_reference&  assign(direction  dir, plane*  pl) noexcept;

  box*      get_box() const noexcept{return m_plane->m_box;}
  plane*  get_plane() const noexcept{return m_plane       ;}

  int  get_flags() const noexcept;

};


class
plane_reference_stack
{
  plane_reference*  m_top    =nullptr;
  plane_reference*  m_current=nullptr;

public:
  plane_reference_stack() noexcept{}
  plane_reference_stack(const plane_reference_stack&   rhs) noexcept=delete;
  plane_reference_stack(      plane_reference_stack&&  rhs) noexcept=delete;
 ~plane_reference_stack(){assign(nullptr,0);}

  plane_reference_stack&  operator=(const plane_reference_stack&   rhs) noexcept=delete;
  plane_reference_stack&  operator=(      plane_reference_stack&&  rhs) noexcept=delete;

  plane_reference&  get_from_top(int  i) const noexcept{return m_top[i];}

  void  assign(const plane_reference*  refs, int  n) noexcept;

  void  seek() noexcept;

  plane_reference&      get_top() const noexcept{return *m_top;}
  plane_reference&  get_current() const noexcept{return *m_current;}

  void  render(const gbstd::canvas&  cv) noexcept;

};


class
view
{
  point3d  m_base;

  int  m_width =0;
  int  m_height=0;

};


class
stack_map
{
  plane_reference_stack*  m_table=nullptr;

  int  m_width =0;
  int  m_height=0;

  int  m_image_width =0;
  int  m_image_height=0;

public:
  ~stack_map(){clear();}

  void  clear() noexcept;

  stack_map&  assign(space_view  sv) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int  get_image_width()  const noexcept{return m_image_width ;}
  int  get_image_height() const noexcept{return m_image_height;}

        plane_reference_stack&  get_stack(int  x, int  y)       noexcept{return m_table[(m_width*y)+x];}
  const plane_reference_stack&  get_stack(int  x, int  y) const noexcept{return m_table[(m_width*y)+x];}

  void  render(const gbstd::canvas&  cv) noexcept;

};




class
space_handler
{
  stack_map  m_front_map;
  stack_map   m_back_map;
  stack_map   m_left_map;
  stack_map  m_right_map;

  direction  m_dir=direction::front;

  stack_map*  m_current_map=nullptr;

public:
  space_handler&  assign(space&  sp) noexcept;

  direction  get_direction(              ) const noexcept{return m_dir;}
  void       set_direction(direction  dir)       noexcept;

  const stack_map&  get_stack_map(direction  dir) const noexcept;
  const stack_map&  get_stack_map() const noexcept{return *m_current_map;}

  void  render(const gbstd::canvas&  cv) noexcept;

};




}



#endif




