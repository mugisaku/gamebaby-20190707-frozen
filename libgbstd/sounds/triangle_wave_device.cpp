#include"libgbstd/sound.hpp"




namespace gbstd{




f32_t
triangle_wave_device::
get_sample() noexcept
{
  auto  v = m_sample_current;

    if(is_downward()){m_sample_current -= m_sample_increment;}
  else               {m_sample_current += m_sample_increment;}

  return v;
}


void
triangle_wave_device::
update(f32_t  number_of_samples_per_cycle) noexcept
{
  auto  half = number_of_samples_per_cycle/2;

  set_number_of_upward_samples(  half);
  set_number_of_downward_samples(half);

  m_sample_current   = -get_volume();
  m_sample_increment = (get_volume()*2)/half;
}




}




