#include"libsubiso/subiso.hpp"




namespace subiso{




box_view&
box_view::
assign(subiso::box&  box, direction  dir) noexcept
{
  m_box = &box;
  m_dir =  dir;

  return *this;
}


plane&  box_view::get_front_plane() const noexcept{return pl(m_dir  );}
plane&  box_view::get_right_plane() const noexcept{return pl(m_dir+1);}
plane&   box_view::get_back_plane() const noexcept{return pl(m_dir+2);}
plane&   box_view::get_left_plane() const noexcept{return pl(m_dir+3);}

box*  box_view::get_front_box() const noexcept{return m(  (m_dir*2));}
box*  box_view::get_right_box() const noexcept{return m(2+(m_dir*2));}
box*   box_view::get_back_box() const noexcept{return m(4+(m_dir*2));}
box*   box_view::get_left_box() const noexcept{return m(6+(m_dir*2));}

box*   box_view::get_back_down_box()  const noexcept{return l(4+(m_dir*2));}
box*   box_view::get_back_up_box()    const noexcept{return u(4+(m_dir*2));}
box*   box_view::get_front_down_box() const noexcept{return l(  (m_dir*2));}
box*   box_view::get_front_up_box()   const noexcept{return u(  (m_dir*2));}

box*  box_view::get_left_up_box()         const noexcept{return u(6+(m_dir*2));}
box*  box_view::get_left_down_box()       const noexcept{return l(6+(m_dir*2));}
box*  box_view::get_left_back_box()       const noexcept{return m(5+(m_dir*2));}
box*  box_view::get_left_back_down_box()  const noexcept{return l(5+(m_dir*2));}
box*  box_view::get_left_back_up_box()    const noexcept{return u(5+(m_dir*2));}
box*  box_view::get_left_front_box()      const noexcept{return m(7+(m_dir*2));}
box*  box_view::get_left_front_down_box() const noexcept{return l(7+(m_dir*2));}
box*  box_view::get_left_front_up_box()   const noexcept{return u(7+(m_dir*2));}

box*  box_view::get_right_up_box()         const noexcept{return u(2+(m_dir*2));}
box*  box_view::get_right_down_box()       const noexcept{return l(2+(m_dir*2));}
box*  box_view::get_right_back_box()       const noexcept{return m(3+(m_dir*2));}
box*  box_view::get_right_back_down_box()  const noexcept{return l(3+(m_dir*2));}
box*  box_view::get_right_back_up_box()    const noexcept{return u(3+(m_dir*2));}
box*  box_view::get_right_front_box()      const noexcept{return m(1+(m_dir*2));}
box*  box_view::get_right_front_down_box() const noexcept{return l(1+(m_dir*2));}
box*  box_view::get_right_front_up_box()   const noexcept{return u(1+(m_dir*2));}




}




