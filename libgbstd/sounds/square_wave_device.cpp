#include"libgbstd/sound.hpp"




namespace gbstd{




void
square_wave_device::
reset() noexcept
{
  sound_device::reset();

  m_low_phase = false;
}


void
square_wave_device::
on_frequency_changed()
{
  auto  number_of_samples_per_cycle = g_number_of_samples_per_second/get_frequency();

    if(number_of_samples_per_cycle <= 0.0)
    {
      throw invalid_number_of_samples_per_cycle();
    }


  m_number_of_samples_per_high = static_cast<uint32_t>(number_of_samples_per_cycle/2);
  m_number_of_samples_per_low  = static_cast<uint32_t>(number_of_samples_per_cycle/2);

    if(!m_number_of_samples_per_high ||
       !m_number_of_samples_per_low)
    {
      throw invalid_number_of_samples_per_cycle();
    }
}


void
square_wave_device::
generate_for_number_of_samples(uint32_t  num_samples, sample_t*  buffer)
{
  check_frequency();

    while(num_samples && !is_slept())
    {
        if(m_low_phase)
        {
            if(m_number_of_remain_samples <= num_samples)
            {
              num_samples -= m_number_of_remain_samples;

                while(m_number_of_remain_samples--)
                {
                  put(-get_volume(),*buffer++);
                }


              m_low_phase = false;

              m_number_of_remain_samples = m_number_of_samples_per_high;
            }

          else
            {
              m_number_of_remain_samples -= num_samples;

                while(num_samples--)
                {
                  put(-get_volume(),*buffer++);
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
                  put(get_volume(),*buffer++);
                }


              m_low_phase = true;

              m_number_of_remain_samples = m_number_of_samples_per_low;
            }

          else
            {
              m_number_of_remain_samples -= num_samples;

                while(num_samples--)
                {
                  put(get_volume(),*buffer++);
                }


              return;
            }
        }
    }
}


}




