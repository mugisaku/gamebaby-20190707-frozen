#include"libgbstd/sound.hpp"




namespace gbstd{




f32_t
sine_wave_device::
get_sample() noexcept
{
  auto  i = static_cast<int>(m_counter);

    if(i >= g_cos_table_length)
    {
      i = g_cos_table_length-(1+(i-g_cos_table_length));
    }


  auto  rate = g_cos_table[i];

  auto  v = get_volume()*rate;

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
  m_increment = (g_cos_table_length*2)/(number_of_samples_per_cycle);
}




}




