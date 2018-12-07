#include"libsubiso/subiso.hpp"




namespace subiso{




void
flag_timer::
set_timer(uint32_t  ms) noexcept
{
  m_next_time = gbstd::g_time+ms;

  m_data |= flags::active;
}


void
flag_timer::
update() noexcept
{
    if(is_active())
    {
        if(gbstd::g_time >= m_next_time)
        {
          set_flag();
        }
    }
}




}




