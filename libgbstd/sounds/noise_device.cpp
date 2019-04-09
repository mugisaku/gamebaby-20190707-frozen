#include"libgbstd/sound.hpp"




namespace gbstd{




void
noise_device::
update() noexcept
{
  auto  num_samples = get_sampling_rate()/get_frequency()/8;

  set_number_of_upward_samples(  num_samples/2);
  set_number_of_downward_samples(num_samples/2);

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




