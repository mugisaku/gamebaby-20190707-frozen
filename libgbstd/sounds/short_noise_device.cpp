#include"libgbstd/sound.hpp"




namespace gbstd{




void
short_noise_device::
generate_for_number_of_samples(uint32_t  n, sample_t*  buffer)
{
    while(!is_slept() && n--)
    {
      m_seed += m_seed + (((m_seed>>6)^
                           (m_seed>>5))&1);

      int  sample = static_cast<int16_t>(m_seed);

      put(sample/32767.0*get_volume(),*buffer++);
    }
}


}




