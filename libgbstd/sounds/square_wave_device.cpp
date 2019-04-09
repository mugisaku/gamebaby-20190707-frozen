#include"libgbstd/sound.hpp"




namespace gbstd{




void
square_wave_device::
update() noexcept
{
  auto  number_of_cycle_samples = get_sampling_rate()/get_frequency();

  set_number_of_upward_samples(  number_of_cycle_samples/2);
  set_number_of_downward_samples(number_of_cycle_samples/2);
}




}




