#include"libgbstd/sound.hpp"




namespace gbstd{




f32_t
sawtooth_wave_device::
get_sample() noexcept
{
  auto  v = m_sample_current                      ;
            m_sample_current += m_sample_increment;

  return v;
}


void
sawtooth_wave_device::
update(f32_t  number_of_samples_per_cycle) noexcept
{
  set_number_of_upward_samples(number_of_samples_per_cycle);
  set_number_of_downward_samples(0);

  m_sample_current   = -get_volume();
  m_sample_increment = (get_volume()*2)/number_of_samples_per_cycle;
}




}




