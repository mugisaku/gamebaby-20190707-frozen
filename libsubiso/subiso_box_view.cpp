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


plane&  box_view::get_front_plane() const noexcept{return pl(is_front()? 0:is_right()? 1:is_back()? 2:3);}
plane&  box_view::get_right_plane() const noexcept{return pl(is_front()? 1:is_right()? 2:is_back()? 3:0);}
plane&   box_view::get_back_plane() const noexcept{return pl(is_front()? 2:is_right()? 3:is_back()? 0:1);}
plane&   box_view::get_left_plane() const noexcept{return pl(is_front()? 3:is_right()? 0:is_back()? 1:2);}

box*  box_view::get_front_box() const noexcept{return m(is_front()? 0:is_right()? 2:is_back()? 4:6);}
box*  box_view::get_right_box() const noexcept{return m(is_front()? 2:is_right()? 4:is_back()? 6:0);}
box*   box_view::get_back_box() const noexcept{return m(is_front()? 4:is_right()? 6:is_back()? 0:2);}
box*   box_view::get_left_box() const noexcept{return m(is_front()? 6:is_right()? 0:is_back()? 2:4);}

box*   box_view::get_back_down_box()  const noexcept{return l(is_front()? 4:is_right()? 6:is_back()? 0:2);}
box*   box_view::get_back_up_box()    const noexcept{return u(is_front()? 4:is_right()? 6:is_back()? 0:2);}
box*   box_view::get_front_down_box() const noexcept{return l(is_front()? 0:is_right()? 2:is_back()? 4:6);}
box*   box_view::get_front_up_box()   const noexcept{return u(is_front()? 0:is_right()? 2:is_back()? 4:6);}

box*  box_view::get_left_up_box()         const noexcept{return u(is_front()? 6:is_right()? 0:is_back()? 2:4);}
box*  box_view::get_left_down_box()       const noexcept{return l(is_front()? 6:is_right()? 0:is_back()? 2:4);}
box*  box_view::get_left_back_box()       const noexcept{return m(is_front()? 5:is_right()? 7:is_back()? 1:3);}
box*  box_view::get_left_back_down_box()  const noexcept{return l(is_front()? 5:is_right()? 7:is_back()? 1:3);}
box*  box_view::get_left_back_up_box()    const noexcept{return u(is_front()? 5:is_right()? 7:is_back()? 1:3);}
box*  box_view::get_left_front_box()      const noexcept{return m(is_front()? 7:is_right()? 1:is_back()? 3:5);}
box*  box_view::get_left_front_down_box() const noexcept{return l(is_front()? 7:is_right()? 1:is_back()? 3:5);}
box*  box_view::get_left_front_up_box()   const noexcept{return u(is_front()? 7:is_right()? 1:is_back()? 3:5);}

box*  box_view::get_right_up_box()         const noexcept{return u(is_front()? 2:is_right()? 4:is_back()? 6:0);}
box*  box_view::get_right_down_box()       const noexcept{return l(is_front()? 2:is_right()? 4:is_back()? 6:0);}
box*  box_view::get_right_back_box()       const noexcept{return m(is_front()? 3:is_right()? 5:is_back()? 7:0);}
box*  box_view::get_right_back_down_box()  const noexcept{return l(is_front()? 3:is_right()? 5:is_back()? 7:0);}
box*  box_view::get_right_back_up_box()    const noexcept{return u(is_front()? 3:is_right()? 5:is_back()? 7:0);}
box*  box_view::get_right_front_box()      const noexcept{return m(is_front()? 1:is_right()? 3:is_back()? 5:7);}
box*  box_view::get_right_front_down_box() const noexcept{return l(is_front()? 1:is_right()? 3:is_back()? 5:7);}
box*  box_view::get_right_front_up_box()   const noexcept{return u(is_front()? 1:is_right()? 3:is_back()? 5:7);}




}




