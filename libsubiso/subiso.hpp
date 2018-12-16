#ifndef subiso_hpp_is_included
#define subiso_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include<list>



namespace subiso{


constexpr int  g_plane_size = 24;

constexpr int     top_flag = 1;
constexpr int  bottom_flag = 2;
constexpr int    left_flag = 4;
constexpr int   right_flag = 8;

namespace edge_flags{
constexpr int   front = 8;
constexpr int   right = 4;
constexpr int    left = 2;
constexpr int    back = 1;

constexpr int   right_front = 8;
constexpr int    right_back = 4;
constexpr int     left_back = 2;
constexpr int    left_front = 1;
}


constexpr int
get_shifted_flags(int  flags, int  n) noexcept
{
  return ((flags<<(4-(n&3)))|((flags&15)>>(n&3)))&15;
}


class
direction
{
  static constexpr int  m_mask_value = 3;

  int  m_value;

public:
  constexpr direction(int  v=0) noexcept: m_value(v&m_mask_value){}

  constexpr operator int() const noexcept{return m_value&m_mask_value;}

  constexpr bool  operator==(direction  d) const noexcept{return (m_value&m_mask_value) == (d.m_value&m_mask_value);}
  constexpr bool  operator!=(direction  d) const noexcept{return (m_value&m_mask_value) != (d.m_value&m_mask_value);}

  constexpr bool  operator==(int  i) const noexcept{return (m_value&m_mask_value) == (i&m_mask_value);}
  constexpr bool  operator!=(int  i) const noexcept{return (m_value&m_mask_value) != (i&m_mask_value);}

  constexpr direction  operator+(direction  d) const noexcept{return direction(m_value+d.m_value);}
  constexpr direction  operator-(direction  d) const noexcept{return direction(m_value-d.m_value);}
  constexpr direction  operator+(int        i) const noexcept{return direction(m_value+i        );}
  constexpr direction  operator-(int        i) const noexcept{return direction(m_value-i        );}
  constexpr direction  operator~(            ) const noexcept{return direction(m_value+2        );}

  direction&  operator+=(direction  d) noexcept{  m_value += d.m_value;  return *this;}
  direction&  operator-=(direction  d) noexcept{  m_value -= d.m_value;  return *this;}
  direction&  operator+=(int        i) noexcept{  m_value +=         i;  return *this;}
  direction&  operator-=(int        i) noexcept{  m_value -=         i;  return *this;}

  direction&  operator++() noexcept{  ++m_value;  return *this;}
  direction&  operator--() noexcept{  --m_value;  return *this;}

  direction  operator++(int) noexcept{  auto  t = *this;  ++m_value;  return t;}
  direction  operator--(int) noexcept{  auto  t = *this;  --m_value;  return t;}

};




namespace directions{
constexpr direction  front = direction(0);
constexpr direction  right = direction(1);
constexpr direction   back = direction(2);
constexpr direction   left = direction(3);
}


inline constexpr const char*
get_name(direction  d) noexcept
{
  return (d == directions::front)? "front"
        :(d == directions::right)? "right"
        :(d == directions::back )? "back"
        :(d == directions::left )? "left"
        :                          "unknown";
}


inline void  print(direction  d) noexcept{printf("%s",get_name(d));}



struct
point3d
{
  int  x;
  int  y;
  int  z;

  constexpr point3d(int  x_=0, int  y_=0, int  z_=0) noexcept: x(x_), y(y_), z(z_){}

  constexpr operator bool() const noexcept{return x && y && z;}

  constexpr bool  operator==(const point3d&  rhs) const noexcept
  {
    return (x == rhs.x) &&
           (y == rhs.y) &&
           (z == rhs.z);
  }

  constexpr bool  operator!=(const point3d&  rhs) const noexcept
  {
    return !(*this == rhs);
  }

  constexpr point3d  operator+(const point3d&  rhs) const noexcept
  {
    return {x+rhs.x,y+rhs.y,z+rhs.z};
  }

  constexpr point3d  operator-(const point3d&  rhs) const noexcept
  {
    return {x-rhs.x,y-rhs.y,z-rhs.z};
  }

  constexpr point3d  operator-() const noexcept
  {
    return {-x,-y,-z};
  }

  point3d&  operator+=(const point3d&  rhs) noexcept
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
  }

  point3d&  operator-=(const point3d&  rhs) noexcept
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
  }

  point3d&  assign(int  x_, int  y_, int  z_) noexcept
  {
    x = x_;
    y = y_;
    z = z_;

    return *this;
  }

  void  print() const noexcept{printf("{%3d,%3d,%3d}",x,y,z);}

};


inline constexpr gbstd::point
transform(gbstd::point  pt, direction  dir) noexcept
{
  return (dir == directions::front)? pt
        :(dir == directions::right)? gbstd::point( pt.y,-pt.x)
        :(dir == directions::back )? gbstd::point(-pt.x,-pt.y)
        :(dir == directions::left )? gbstd::point(-pt.y, pt.x)
        :                            gbstd::point(    0,    0);
}


class space;
class box;
class plane;
class actor;
class space_handler;




class
plane
{
  box*  m_box=nullptr;

  int  m_index=0;

  enum class kind{
     top,
    side,
  } m_kind;


  plane(subiso::box&  box, kind  k, int  i) noexcept: m_box(&box), m_kind(k), m_index(i){}

public:
  static plane  make_top( subiso::box&  box        ) noexcept{return plane(box,kind::top ,0);}
  static plane  make_side(subiso::box&  box, int  i) noexcept{return plane(box,kind::side,i);}

  box*  get_box() const noexcept{return m_box;}

  int  get_index() const noexcept{return m_index;}

  bool  is_top()   const noexcept{return m_kind == kind::top;}
  bool  is_side() const noexcept{return m_kind == kind::side;}

  void  render(direction  dir, int  flags, const gbstd::canvas&  cv, int  x, int  y, int  z_base) const noexcept;

};




constexpr int  g_water_value_max = 266;


class
box_link
{
  box*  m_up_box  =nullptr;
  box*  m_down_box=nullptr;

  box*   m_upper_boxes[8];
  box*  m_middle_boxes[8];
  box*   m_lower_boxes[8];

public:
  box*  get_up_box(                 ) const noexcept{return m_up_box      ;}
  void  set_up_box(subiso::box*  box)       noexcept{       m_up_box = box;}

  box*  get_down_box(                 ) const noexcept{return m_down_box      ;}
  void  set_down_box(subiso::box*  box)       noexcept{       m_down_box = box;}

  box*  get_upper_box(                   int  i) const noexcept{return m_upper_boxes[i&7]      ;}
  void  set_upper_box(subiso::box*  box, int  i)       noexcept{       m_upper_boxes[i&7] = box;}

  box*  get_middle_box(                   int  i) const noexcept{return m_middle_boxes[i&7]      ;}
  void  set_middle_box(subiso::box*  box, int  i)       noexcept{       m_middle_boxes[i&7] = box;}

  box*  get_lower_box(                   int  i) const noexcept{return m_lower_boxes[i&7]      ;}
  void  set_lower_box(subiso::box*  box, int  i)       noexcept{       m_lower_boxes[i&7] = box;}

};


class
box: public box_link
{
public:
  enum class kind{
    null,
    earth,
    stairs,

  } m_kind=kind::null;

private:
  space*  m_space=nullptr;

  point3d  m_index;

  plane  m_top_plane;
  plane  m_side_planes[4];

  int     m_top_edge_flags=0;
  int  m_middle_edge_flags=0;
  int  m_bottom_edge_flags=0;

  int  m_distance=0;

  int  m_ground_line=0;

  actor*  m_owner_actor=nullptr;

  direction  m_dir;

  struct flags{
    static constexpr uint32_t        active = 1;
    static constexpr uint32_t  water_source = 2;
    static constexpr uint32_t  water_filled = 4;
  };


  bool  m_check_flag;

  uint32_t  m_state=0;

public:
  box() noexcept;

  operator bool() const noexcept{return m_kind != kind::null;}

  bool  operator==(direction  dir) const noexcept{return m_dir == dir;}
  bool  operator!=(direction  dir) const noexcept{return m_dir != dir;}

  void    check() noexcept{m_check_flag =  true;}
  void  uncheck() noexcept{m_check_flag = false;}
  bool  is_checked() const noexcept{return m_check_flag;}

  bool   test_active_flag() const noexcept{return m_state&flags::active;}
  void    set_active_flag() noexcept{m_state |=  flags::active;}
  void  unset_active_flag() noexcept{m_state &= ~flags::active;}

  bool   test_water_source_flag() const noexcept{return m_state&flags::water_source;}
  void    set_water_source_flag() noexcept{m_state |=  flags::water_source;}
  void  unset_water_source_flag() noexcept{m_state &= ~flags::water_source;}

  bool   test_water_filled_flag() const noexcept{return m_state&flags::water_filled;}
  void    set_water_filled_flag() noexcept{m_state |=  flags::water_filled;}
  void  unset_water_filled_flag() noexcept{m_state &= ~flags::water_filled;}

  space*  get_space(          ) const noexcept{return m_space     ;}
  void    set_space(space*  sp)       noexcept{       m_space = sp;}

  const point3d&  get_index(           ) const noexcept{return m_index     ;}
  void            set_index(point3d  pt)       noexcept{       m_index = pt;}

  int  get_ground_line() const noexcept{return m_ground_line;}

        plane&  get_top_plane()       noexcept{return m_top_plane;}
  const plane&  get_top_plane() const noexcept{return m_top_plane;}

        plane&  get_side_plane(int  i)       noexcept{return m_side_planes[i&3];}
  const plane&  get_side_plane(int  i) const noexcept{return m_side_planes[i&3];}

  actor*   get_owner_actor(         ) const noexcept{return m_owner_actor    ;}
  void     set_owner_actor(actor*  a)       noexcept{       m_owner_actor = a;}

  int   get_distance(      ) const noexcept{return m_distance    ;}
  void  set_distance(int  v)       noexcept{       m_distance = v;}

  direction   get_direction(            ) const noexcept{return m_dir    ;}
  void        set_direction(direction  d)       noexcept{       m_dir = d;}

  int     get_top_edge_flags() const noexcept{return m_top_edge_flags;}
  int  get_middle_edge_flags() const noexcept{return m_middle_edge_flags;}
  int  get_bottom_edge_flags() const noexcept{return m_bottom_edge_flags;}

  void  update_edge_flags() noexcept;
  void  update() noexcept;

  kind  get_kind() const noexcept{return m_kind;}

  void  flow_water() noexcept;

  bool  is_surrounded_by_earth() const noexcept;
  bool  is_surrounded_by_water() const noexcept;
  bool   is_contacted_water() const noexcept;

  void  be_null()  noexcept{m_kind = kind::null;}
  void  be_earth() noexcept{m_kind = kind::earth;}
  void  be_stairs(direction  d) noexcept;

  bool  is_null() const noexcept{return m_kind == kind::null;}
  bool  is_earth() const noexcept{return m_kind == kind::earth;}
  bool  is_stairs() const noexcept{return m_kind == kind::stairs;}

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

  void  update() noexcept;

  void  print() const noexcept;

};




class
box_view
{
  box*  m_box=nullptr;

  direction  m_dir=directions::front;

  box*  u(int  i) const noexcept{return m_box->get_upper_box(i);}
  box*  m(int  i) const noexcept{return m_box->get_middle_box(i);}
  box*  l(int  i) const noexcept{return m_box->get_lower_box(i);}

  int  te() const noexcept{return get_shifted_flags(m_box->get_top_edge_flags()   ,m_dir);}
  int  me() const noexcept{return get_shifted_flags(m_box->get_middle_edge_flags(),m_dir);}
  int  be() const noexcept{return get_shifted_flags(m_box->get_bottom_edge_flags(),m_dir);}

  plane&  pl(int  i) const noexcept{return m_box->get_side_plane(i);}

public:
  box_view() noexcept{}
  box_view(direction  dir) noexcept: m_dir(dir){}
  box_view(subiso::box&  box, direction  dir=directions::front) noexcept{assign(box,dir);}

  box_view&  assign(subiso::box&  box, direction  dir) noexcept;

  bool  is_front() const noexcept{return m_dir == directions::front;}
  bool  is_right() const noexcept{return m_dir == directions::right;}
  bool  is_back()  const noexcept{return m_dir == directions::back;}
  bool  is_left()  const noexcept{return m_dir == directions::left;}

  plane&  get_top_plane() const noexcept{return m_box->get_top_plane();}

  plane&  get_front_plane() const noexcept;
  plane&  get_right_plane() const noexcept;
  plane&   get_back_plane() const noexcept;
  plane&   get_left_plane() const noexcept;

  void  set_box(subiso::box&  box)       noexcept{       m_box = &box;}
  box*  get_box(                 ) const noexcept{return m_box       ;}

  void       set_direction(direction  dir)       noexcept{       m_dir = dir;}
  direction  get_direction(              ) const noexcept{return m_dir      ;}

  bool  test_top_edge(   int  flag) const noexcept{return te()&flag;}
  bool  test_middle_edge(int  flag) const noexcept{return me()&flag;}
  bool  test_bottom_edge(int  flag) const noexcept{return be()&flag;}

  box*     get_up_box() const noexcept{return m_box->get_up_box();}
  box*   get_down_box() const noexcept{return m_box->get_down_box();}

  box*   get_front_box() const noexcept;
  box*   get_right_box() const noexcept;
  box*    get_back_box() const noexcept;
  box*    get_left_box() const noexcept;

  box*   get_front_down_box() const noexcept;
  box*   get_front_up_box()   const noexcept;

  box*   get_right_up_box()         const noexcept;
  box*   get_right_down_box()       const noexcept;
  box*   get_right_back_box()       const noexcept;
  box*   get_right_back_down_box()  const noexcept;
  box*   get_right_back_up_box()    const noexcept;
  box*   get_right_front_box()      const noexcept;
  box*   get_right_front_down_box() const noexcept;
  box*   get_right_front_up_box()   const noexcept;

  box*   get_back_down_box()  const noexcept;
  box*   get_back_up_box()    const noexcept;

  box*   get_left_up_box()         const noexcept;
  box*   get_left_down_box()       const noexcept;
  box*   get_left_back_box()       const noexcept;
  box*   get_left_back_down_box()  const noexcept;
  box*   get_left_back_up_box()    const noexcept;
  box*   get_left_front_box()      const noexcept;
  box*   get_left_front_down_box() const noexcept;
  box*   get_left_front_up_box()   const noexcept;

};


class
space_view
{
  space*  m_space=nullptr;

  int  m_x_length=0;
  int  m_y_length=0;
  int  m_z_length=0;

  direction  m_dir=directions::front;

public:
  space_view() noexcept{}
  space_view(space&  sp, direction  dir=directions::front) noexcept{assign(sp,dir);}

  space_view&  assign(space&  sp, direction  dir=directions::front) noexcept;

  bool  is_front() const noexcept{return m_dir == directions::front;}
  bool  is_right() const noexcept{return m_dir == directions::right;}
  bool  is_back()  const noexcept{return m_dir == directions::back;}
  bool  is_left()  const noexcept{return m_dir == directions::left;}

  void  update_size() noexcept;

  void    set_space(space&  sp)       noexcept{       m_space = &sp;}
  space*  get_space(          ) const noexcept{return m_space      ;}

  void       set_direction(direction  dir)       noexcept{       m_dir = dir;}
  direction  get_direction(              ) const noexcept{return m_dir      ;}

  int  get_x_length() const noexcept{return m_x_length;}
  int  get_y_length() const noexcept{return m_y_length;}
  int  get_z_length() const noexcept{return m_z_length;}

  box&  get_box(int  x, int  y, int  z) const noexcept;

  void  print() const noexcept;

};




struct tops{
  box*  m_left_up_box =nullptr;
  box*  m_back_up_box =nullptr;
  box*  m_right_up_box=nullptr;
  box*  m_front_box   =nullptr;
  box*  m_back_box    =nullptr;
};

struct fronts{
  box*  m_front_down_box =nullptr;
  box*  m_left_front_box =nullptr;
  box*  m_right_front_box=nullptr;
};


class
plane_reference
{
  int  m_image_z_base=0;

  box*  m_left_box =nullptr;
  box*  m_right_box=nullptr;

  union{
    tops      m_tops;
    fronts  m_fronts;
  };

  plane*  m_plane=nullptr;

  void    reset_top_link(const box_view&  bv) noexcept;
  void  reset_front_link(const box_view&  bv) noexcept;

  int    get_flags_of_top_plane(direction  dir) const noexcept;
  int  get_flags_of_front_plane(direction  dir) const noexcept;

public:
  plane_reference() noexcept{}
  plane_reference(direction  dir, plane*  pl) noexcept{assign(dir,pl);}

  plane_reference&  assign(direction  dir, plane*  pl) noexcept;

  box*      get_box() const noexcept{return m_plane->get_box();}
  plane*  get_plane() const noexcept{return m_plane          ;}

  box*  get_left_box()  const noexcept{return  m_left_box;}
  box*  get_right_box() const noexcept{return m_right_box;}

  const tops&    get_tops()   const noexcept{return m_tops;}
  const fronts&  get_fronts() const noexcept{return m_fronts;}

  int  get_flags(direction  dir) const noexcept;

  void  set_image_z_base(int  v)       noexcept{       m_image_z_base = v;}
  int   get_image_z_base(      ) const noexcept{return m_image_z_base    ;}

};


class
plane_reference_stack
{
  plane_reference*  m_top    =nullptr;
  plane_reference*  m_current=nullptr;

  int  m_length=0;

  static plane_reference*  seek(plane_reference*  p) noexcept;

public:
  plane_reference_stack() noexcept{}
  plane_reference_stack(const plane_reference_stack&   rhs) noexcept=delete;
  plane_reference_stack(      plane_reference_stack&&  rhs) noexcept=delete;
 ~plane_reference_stack(){assign(nullptr,0);}

  plane_reference_stack&  operator=(const plane_reference_stack&   rhs) noexcept=delete;
  plane_reference_stack&  operator=(      plane_reference_stack&&  rhs) noexcept=delete;

  plane_reference&  get_from_top(int  i) const noexcept{return m_top[i];}

  void  assign(const plane_reference*  refs, int  n) noexcept;

  void  seek() noexcept{m_current = seek(m_top);}

  int  get_length() const noexcept{return m_length;}

  plane_reference*      get_top() const noexcept{return m_top;}
  plane_reference*  get_current() const noexcept{return m_current;}

  void  render(direction  dir, const gbstd::canvas&  cv, int  x, int  y) noexcept;

};


class
stack_map
{
  plane_reference_stack*  m_table=nullptr;

  int  m_width =0;
  int  m_height=0;

  int  m_image_width =0;
  int  m_image_height=0;

  int  m_upper_image_height=0;
  int  m_lower_image_height=0;

  direction  m_dir;

public:
  ~stack_map(){clear();}

  void  clear() noexcept;

  stack_map&  assign(space_view  sv) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int  get_image_width()  const noexcept{return m_image_width ;}
  int  get_image_height() const noexcept{return m_image_height;}

  int  get_upper_image_height() const noexcept{return m_upper_image_height;}
  int  get_lower_image_height() const noexcept{return m_lower_image_height;}

  direction  get_direction() const noexcept{return m_dir;}

        plane_reference_stack&  get_stack(int  x, int  y)       noexcept{return m_table[(m_width*y)+x];}
  const plane_reference_stack&  get_stack(int  x, int  y) const noexcept{return m_table[(m_width*y)+x];}

  void  render_line_block(int  x_quo, int  y_quo, const gbstd::canvas&  cv, int  x, int  y) noexcept;

  void  render(gbstd::point  view_center_pos, const gbstd::canvas&  cv) noexcept;

  void  print() const noexcept;

  gbstd::point  transform(const point3d&  src) const noexcept;

};




class
stack_mapset
{
  stack_map  m_table[4];

public:
  stack_mapset() noexcept{}
  stack_mapset(space&  sp) noexcept{assign(sp);}

  stack_mapset&  assign(space&  sp) noexcept
  {
    m_table[directions::front].assign(space_view(sp,directions::front));
    m_table[directions::left ].assign(space_view(sp,directions::left ));
    m_table[directions::right].assign(space_view(sp,directions::right));
    m_table[directions::back ].assign(space_view(sp,directions::back ));

    return *this;
  }

        stack_map&  get_stack_map(direction  dir)       noexcept{return m_table[dir];}
  const stack_map&  get_stack_map(direction  dir) const noexcept{return m_table[dir];}

        stack_map&  operator[](direction  dir)       noexcept{return m_table[dir];}
  const stack_map&  operator[](direction  dir) const noexcept{return m_table[dir];}

};




class
move_context
{
  int  m_x;
  int  m_y;
  int  m_z;
  int  m_n;

public:
  constexpr move_context(int  x=0, int  y=0, int  z=0, int  n=0) noexcept:
  m_x(x), m_y(y), m_z(z), m_n(n){}

  constexpr operator bool() const noexcept{return m_n;}

  move_context&  assign(int  x=0, int  y=0, int  z=0, int  n=0) noexcept
  {
    m_x = x;
    m_y = y;
    m_z = z;
    m_n = n;

    return *this;
  }

  void  operator()(point3d&  pt) noexcept
  {
    pt.x += m_x;
    pt.y += m_y;
    pt.z += m_z;

    --m_n;
  }

};




class
flag_timer
{
  struct flags{
    static constexpr int  elapsed = 1;
    static constexpr int   active = 2;
  };

  uint32_t  m_data=0;

  uint32_t  m_next_time=0;

public:
  void  clear() noexcept{m_data = 0;}

  void    set_timer(uint32_t  ms) noexcept;
  void  unset_timer(            ) noexcept{m_data &= ~flags::active;}

  bool  is_active() const noexcept{return m_data&flags::active;}

  void    set_flag() noexcept{m_data |=  flags::elapsed;}
  void  unset_flag() noexcept{m_data &= ~flags::elapsed;}
  bool   test_flag() const noexcept{return m_data&flags::elapsed;}

  void  update() noexcept;

};


struct
actor
{
  space*  m_space=nullptr;

  point3d  m_current_position;
  point3d  m_next_position;

  move_context  m_first_move_context;
  move_context  m_second_move_context;

  gbstd::point  m_transformed_position;

  int  m_image_z=0;

  direction  m_dir=directions::front;
  direction  m_transformed_dir;

  uint32_t  m_next_animation_time=0;

  int  m_animation_counter=0;

  box*  m_next_step_box=nullptr;
  box*  m_current_step_box=nullptr;

  bool  m_dirty_flag;

  flag_timer  m_flag_timer;

  void  transform(const stack_map&  map) noexcept;

  actor() noexcept;

  void    set_space(space*  sp)       noexcept{       m_space = sp;}
  space*  get_space(          ) const noexcept{return m_space     ;}

  void  set_current_step_box(subiso::box*  box)       noexcept;
  box*  get_current_step_box(                 ) const noexcept{return m_current_step_box;}

  direction  get_direction(            ) const noexcept{return m_dir    ;}
  void       set_direction(direction  d)       noexcept{       m_dir = d;}

  bool  is_forward_direction() const noexcept{return m_current_step_box->get_direction() ==  m_dir;}
  bool  is_reverse_direction() const noexcept{return m_current_step_box->get_direction() == ~m_dir;}

        flag_timer&  get_flag_timer()       noexcept{return m_flag_timer;}
  const flag_timer&  get_flag_timer() const noexcept{return m_flag_timer;}

  void  step() noexcept;

  void  render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept;

};


}



#endif




