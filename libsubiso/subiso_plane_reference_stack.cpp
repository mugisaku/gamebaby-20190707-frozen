#include"libsubiso/subiso.hpp"




namespace subiso{




void
plane_reference_stack::
assign(const plane_reference*  refs, int  n) noexcept
{
  delete[] m_top;

  m_length = n;

    if(n)
    {
                  m_top = new plane_reference[n];
      m_current = m_top                         ;

        for(int  i = 0;  i < n;  ++i)
        {
          m_top[i] = refs[i];
        }
    }

  else
    {
      m_top = nullptr;
    }
}


void
plane_reference_stack::
seek() noexcept
{
  m_current = m_top;

    for(;;)
    {
      auto  pl = m_current->get_plane();

        if(!pl)
        {
          break;
        }


        if(!pl->get_box()->is_null())
        {
          break;
        }


      ++m_current;
    }
}


void
plane_reference_stack::
render(const gbstd::canvas&  cv) noexcept
{
  seek();

  auto&  el = get_current();

  auto  pl = el.get_plane();

    if(pl)
    {
      pl->render(el.get_flags(),cv,el.get_image_z_base());
    }
}




}




