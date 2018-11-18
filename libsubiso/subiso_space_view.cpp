#include"libsubiso/subiso.hpp"




namespace subiso{




space_view&
space_view::
assign(space&  sp, direction  dir) noexcept
{
  m_space = &sp;
  m_dir   = dir;

  update_size();

  return *this;
}


void
space_view::
update_size() noexcept
{
    if((m_dir == direction::front) ||
       (m_dir == direction::back ))
    {
      m_x_length = m_space->get_x_length();
      m_y_length = m_space->get_y_length();
    }

  else
//    if((m_dir == direction::left ) ||
//       (m_dir == direction::right))
    {
      m_x_length = m_space->get_y_length();
      m_y_length = m_space->get_x_length();
    }


  m_z_length = m_space->get_z_length();
}


box&
space_view::
get_box(int  x, int  y, int  z) const noexcept
{
    switch(m_dir)
    {
  case(direction::front): return m_space->get_box(    x,y,z);
  case(direction::left ): return m_space->get_box_ry( y,x,z);
  case(direction::back ): return m_space->get_box_rxy(x,y,z);
  case(direction::right): return m_space->get_box_rx( y,x,z);
    }


  static box  null;

  return null;
}


void
space_view::
print() const noexcept
{
  printf("{\n");

    for(int  z = m_z_length-1;  z >= 0;  --z)
    {
      printf("{\n");

        for(int  y = m_y_length-1;  y >= 0;  --y)
        {
            for(int  x = 0;  x < m_x_length;  ++x)
            {
              auto&  box = get_box(x,y,z);

              box.m_index.print();
            }


          printf("\n");
        }


      printf("}\n");
    }


  printf("}\n");
}




}




