#include"libgbstd/sound.hpp"




namespace gbstd{




void
noise_device::
reset() noexcept
{
  m_seed = 0xFFFF;

  m_result = 1;
}


void
noise_device::
generate_for_time(uint32_t  milsec, sample_t*  buffer) noexcept
{
  generate_for_time(g_number_of_samples_per_millisecond*milsec,buffer);
}


void
noise_device::
generate_for_number_of_samples(uint32_t  n, sample_t*  buffer) noexcept
{
  int  v = 0;

    while(n--)
    {
        if(!m_seed)
        {
          m_seed = 1;
        }


      m_seed += m_seed + (((m_seed>>(m_shortspan_flag? 6:14))^
                           (m_seed>>(m_shortspan_flag? 5:13)))&1);
      m_result ^= m_seed&1;

//      v = m_result? m_volume:-m_volume;


      *buffer++ += (v<<16);
    }
}


}




