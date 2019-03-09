#include"libgbstd/sound.hpp"




namespace gbstd{




square_wave_device&
square_wave_device::
assign(f32_t  freq, sample_t  vol)
{
  set_frequency(freq);
  set_volume(    vol);

  reset();

  return *this;
}


void
square_wave_device::
reset() noexcept
{
  sound_device::reset();

  update_parameters();

  m_low_phase = false;
}


void
square_wave_device::
update_parameters()
{
  m_number_of_samples_per_high = static_cast<uint32_t>(m_number_of_samples_per_cycle/2);
  m_number_of_samples_per_low  = static_cast<uint32_t>(m_number_of_samples_per_cycle/2);

    if(!m_number_of_samples_per_high ||
       !m_number_of_samples_per_low)
    {
      throw invalid_number_of_samples_per_cycle();
    }
}


void
square_wave_device::
check_frequency()
{
    if(m_status.test(flags::frequency_changed))
    {
      m_status.unset(flags::frequency_changed);

      update_parameters();
    }
}


void
square_wave_device::
generate_for_number_of_samples(uint32_t  num_samples, sample_t*  buffer)
{
    while(num_samples && !is_slept())
    {
        if(m_low_phase)
        {
            if(m_number_of_remain_samples <= num_samples)
            {
              num_samples -= m_number_of_remain_samples;

                while(m_number_of_remain_samples--)
                {
                  put(-m_volume,*buffer++);

                  check_frequency();
                }


              m_low_phase = false;

              m_number_of_remain_samples = m_number_of_samples_per_high;
            }

          else
            {
              m_number_of_remain_samples -= num_samples;

                while(num_samples--)
                {
                  put(-m_volume,*buffer++);

                  check_frequency();
                }


              return;
            }
        }

      else
        {
            if(m_number_of_remain_samples <= num_samples)
            {
              num_samples -= m_number_of_remain_samples;

                while(m_number_of_remain_samples--)
                {
                  put(m_volume,*buffer++);

                  check_frequency();
                }


              m_low_phase = true;

              m_number_of_remain_samples = m_number_of_samples_per_low;
            }

          else
            {
              m_number_of_remain_samples -= num_samples;

                while(num_samples--)
                {
                  put(m_volume,*buffer++);

                  check_frequency();
                }


              return;
            }
        }
    }
}


}




