#include"libgbstd/sound.hpp"




namespace gbstd{




noise_device&
noise_device::
assign(sample_t  vol)
{
  set_volume(vol);

  reset();

  return *this;
}


void
noise_device::
reset() noexcept
{
  sound_device::reset();

  m_seed = 0xFFFF;
}


void
noise_device::
generate_for_number_of_samples(uint32_t  n, sample_t*  buffer)
{
    while(n--)
    {
      m_seed += m_seed + (((m_seed>>14)^
                           (m_seed>>13))&1);


      int  sample = static_cast<int16_t>(m_seed);

      put(sample/32767.0*m_volume,*buffer++);
    }
}


}




