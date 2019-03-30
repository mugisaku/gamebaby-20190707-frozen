#include"libgbstd/sound.hpp"




namespace gbstd{




void
noise_device::
reset() noexcept
{
  sound_device::reset();

  m_seed = 0xFFFF;
}


void
noise_device::
on_frequency_changed()
{
  m_number_of_samples_per_cycle = static_cast<uint32_t>(g_number_of_samples_per_second/get_frequency()/8);
printf("%u\n",m_number_of_samples_per_cycle);

    if(!m_number_of_samples_per_cycle)
    {
      m_number_of_samples_per_cycle = 1;
    }


  m_number_of_remain_samples = m_number_of_samples_per_cycle;
}


void
noise_device::
update_seed() noexcept
{
  m_seed += m_seed + (((m_seed>>14)^
                       (m_seed>>13))&1);
}


void
noise_device::
generate_for_number_of_samples(uint32_t  n, sample_t*  buffer)
{
    if(!m_number_of_samples_per_cycle)
    {
      m_number_of_samples_per_cycle = 1;
    }


    while(!is_slept() && n--)
    {
        if(!m_number_of_remain_samples)
        {
          m_number_of_remain_samples = m_number_of_samples_per_cycle;

          update_seed();
        }

      else
        {
          --m_number_of_remain_samples;
        }


      int  sample = static_cast<int16_t>(m_seed);

      put(sample/32767.0*get_volume(),*buffer++);
    }
}


}




