#include"libgbstd/sound.hpp"




namespace gbstd{




f32_t
sine_wave_device::
get_sample() noexcept
{
  auto  v = get_volume()*g_cos_table[static_cast<int>(m_counter)];

  m_counter += m_increment;

  return v;
}


void
sine_wave_device::
update(f32_t  number_of_samples_per_cycle) noexcept
{
  set_number_of_upward_samples(  number_of_samples_per_cycle/2);
  set_number_of_downward_samples(number_of_samples_per_cycle/2);

  m_counter = 0;
  m_increment = g_cos_table_length/number_of_samples_per_cycle;
}




}




