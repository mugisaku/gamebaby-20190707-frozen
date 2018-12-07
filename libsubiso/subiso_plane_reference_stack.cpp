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


plane_reference*
plane_reference_stack::
seek(plane_reference*  p) noexcept
{
    for(;;)
    {
      auto  pl = p->get_plane();

        if(!pl)
        {
          break;
        }


      auto&  box = *pl->get_box();

        if(!box.is_null() || box.test_water_filled_flag())
        {
          break;
        }


      ++p;
    }


  return p;
}


void
plane_reference_stack::
render(direction  dir, const gbstd::canvas&  cv) noexcept
{
  seek();

  auto  el = get_current();

    if(el)
    {
      auto  pl = el->get_plane();

        if(pl)
        {
          auto&  box = *pl->get_box();

            if(box.is_stairs() && (box != dir))
            {
              auto  elel = seek(el+1);

                if(elel)
                {
                  auto  plpl = elel->get_plane();

                    if(plpl)
                    {
                      plpl->render(dir,elel->get_flags(dir),cv,elel->get_image_z_base());
                    }
                }
            }


          pl->render(dir,el->get_flags(dir),cv,el->get_image_z_base());
        }
    }
}




}




