#include"libgbstd/sound.hpp"




namespace gbstd{




void
sound_device::
reset() noexcept
{
  m_number_of_samples_per_cycle = g_number_of_samples_per_second/m_frequency;

  m_number_of_samples_per_vm_step        = 0;
  m_number_of_remain_samples_for_vm_step = 0;
  m_number_of_vm_steps                   = 0;

  m_number_of_samples_per_fm_step        = 0;
  m_number_of_remain_samples_for_fm_step = 0;
  m_number_of_fm_steps                   = 0;
}




bool
sound_device::
process_fm() noexcept
{
    if(m_number_of_fm_steps)
    {
        if(!--m_number_of_remain_samples_for_fm_step)
        {
          --m_number_of_fm_steps;

          m_number_of_remain_samples_for_fm_step = m_number_of_samples_per_fm_step;

          m_frequency += m_fm_increment;

          return true;
        }
    }


  return false;
}


bool
sound_device::
process_vm() noexcept
{
    if(m_number_of_vm_steps)
    {
        if(!--m_number_of_remain_samples_for_vm_step)
        {
          --m_number_of_vm_steps;

          m_number_of_remain_samples_for_vm_step = m_number_of_samples_per_vm_step;

          m_volume += m_vm_increment;

          return true;
        }
    }


  return false;
}


void
sound_device::
put(sample_t  src, sample_t&  dst) noexcept
{
  dst = src;
}




void
sound_device::
apply_absolute_fm(f32_t  target_freq, int  num_steps, uint32_t  ms) noexcept
{
  apply_relative_fm(m_frequency-target_freq,num_steps,ms);
}


void
sound_device::
apply_relative_fm(f32_t  freq, int  num_steps, uint32_t  ms) noexcept
{
    if(num_steps)
    {
      m_fm_increment = std::abs(freq/num_steps);

        if(m_frequency > freq)
        {
          m_fm_increment = -m_fm_increment;
        }


      m_number_of_fm_steps = num_steps;

      auto&  num_samples    = m_number_of_samples_per_fm_step;
      auto&  num_remsamples = m_number_of_remain_samples_for_fm_step;

                       num_samples = g_number_of_samples_per_millisecond*ms/num_steps;
      num_remsamples = num_samples                                                   ;
    }
}


void
sound_device::
apply_absolute_vm(sample_t  target_vol, int  num_steps, uint32_t  ms) noexcept
{
  apply_relative_vm(m_volume-target_vol,num_steps,ms);
}


void
sound_device::
apply_relative_vm(sample_t  vol, int  num_steps, uint32_t  ms) noexcept
{
    if(num_steps)
    {
      m_vm_increment = std::abs(vol/num_steps);

        if(m_volume > vol)
        {
          m_vm_increment = -m_vm_increment;
        }


      m_number_of_vm_steps = num_steps;

      auto&  num_samples    = m_number_of_samples_per_vm_step;
      auto&  num_remsamples = m_number_of_remain_samples_for_vm_step;

                       num_samples = g_number_of_samples_per_millisecond*ms/num_steps;
      num_remsamples = num_samples                                                   ;
    }
}




}




