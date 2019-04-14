#include"libgbstd/sound.hpp"




namespace gbstd{




void
noise_device::
update(f32_t  number_of_samples_per_cycle) noexcept
{
  number_of_samples_per_cycle /= 8;

  set_number_of_upward_samples(  number_of_samples_per_cycle/2);
  set_number_of_downward_samples(number_of_samples_per_cycle/2);

  update_seed();
}


void
noise_device::
update_seed() noexcept
{
  m_seed += m_seed + (((m_seed>>14)^
                       (m_seed>>13))&1);
}




}




