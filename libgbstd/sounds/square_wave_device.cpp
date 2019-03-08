#include"libgbstd/sound.hpp"




namespace gbstd{




void
square_wave_device::
reset() noexcept
{
  sound_device::reset();

  m_number_of_samples_per_high = static_cast<uint32_t>(m_number_of_samples_per_cycle/2);
  m_number_of_samples_per_low  = static_cast<uint32_t>(m_number_of_samples_per_cycle/2);

  m_number_of_remain_samples  = m_number_of_samples_per_high;

  m_low_phase = false;
}


void
square_wave_device::
update_parameters() noexcept
{
  m_number_of_samples_per_cycle = g_number_of_samples_per_second/m_frequency;

  m_number_of_samples_per_high = static_cast<uint32_t>(m_number_of_samples_per_cycle/2);
  m_number_of_samples_per_low  = static_cast<uint32_t>(m_number_of_samples_per_cycle/2);
}


void
square_wave_device::
generate_for_time(uint32_t  milsec, sample_t*  buffer) noexcept
{
  generate_for_time(g_number_of_samples_per_millisecond*milsec,buffer);
}


void
square_wave_device::
check_frequency() noexcept
{
    if(m_status.test(flags::frequency_changed))
    {
      m_status.unset(flags::frequency_changed);

      update_parameters();
    }
}


void
square_wave_device::
generate_for_number_of_samples(uint32_t  num_samples, sample_t*  buffer) noexcept
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




