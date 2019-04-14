#include"libgbstd/sound.hpp"




namespace gbstd{




void
square_wave_device::
update(f32_t  number_of_samples_per_cycle) noexcept
{
  set_number_of_upward_samples(  number_of_samples_per_cycle/2);
  set_number_of_downward_samples(number_of_samples_per_cycle/2);
}




}




