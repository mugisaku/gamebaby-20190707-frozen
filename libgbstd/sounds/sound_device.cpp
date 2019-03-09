#include"libgbstd/sound.hpp"




namespace gbstd{




sound_device&
sound_device::
assign(f32_t  freq, sample_t  vol)
{
  set_frequency(freq);
  set_volume(    vol);

  reset();

  return *this;
}


void
sound_device::
reset() noexcept
{
  m_status.clear();

  wake();

  m_number_of_remain_samples_for_timer = 0;

  m_number_of_samples_per_vm_step        = 0;
  m_number_of_remain_samples_for_vm_step = 0;
  m_number_of_vm_steps                   = 0;

  m_number_of_samples_per_fm_step        = 0;
  m_number_of_remain_samples_for_fm_step = 0;
  m_number_of_fm_steps                   = 0;
}




void
sound_device::
set_timer_callback(void(*callback)(sound_device&,void*), void*  data) noexcept
{
  m_timer_callback      = callback;
  m_timer_callback_data = data;
}


void
sound_device::
set_fm_callback(void(*callback)(sound_device&,void*), void*  data) noexcept
{
  m_fm_callback      = callback;
  m_fm_callback_data = data;
}


void
sound_device::
set_vm_callback(void(*callback)(sound_device&,void*), void*  data) noexcept
{
  m_vm_callback      = callback;
  m_vm_callback_data = data;
}




void
sound_device::
process_timer() noexcept
{
    if(m_number_of_remain_samples_for_timer)
    {
        if(!--m_number_of_remain_samples_for_timer)
        {
          sleep();

            if(m_timer_callback)
            {
              m_timer_callback(*this,m_timer_callback_data);
            }
        }
    }
}


void
sound_device::
process_fm() noexcept
{
    if(m_number_of_fm_steps)
    {
        if(!--m_number_of_remain_samples_for_fm_step)
        {
            if(!--m_number_of_fm_steps)
            {
                if(m_fm_callback)
                {
                  m_fm_callback(*this,m_fm_callback_data);
                }
            }


          m_number_of_remain_samples_for_fm_step = m_number_of_samples_per_fm_step;

          m_frequency += m_fm_increment;

          m_status.set(flags::frequency_changed);
        }
    }
}


void
sound_device::
process_vm() noexcept
{
    if(m_number_of_vm_steps)
    {
        if(!--m_number_of_remain_samples_for_vm_step)
        {
            if(!--m_number_of_vm_steps)
            {
                if(m_vm_callback)
                {
                  m_vm_callback(*this,m_vm_callback_data);
                }
            }


          m_number_of_remain_samples_for_vm_step = m_number_of_samples_per_vm_step;

          m_volume += m_vm_increment;

          m_status.set(flags::volume_changed);
        }
    }
}


void
sound_device::
put(sample_t  src, sample_t&  dst) noexcept
{
    if(!is_muted())
    {
      dst += src;
    }


  process_fm();
  process_vm();
  process_timer();
}




void
sound_device::
set_frequency(f32_t  freq)
{
    if(freq <= 0.0)
    {
      throw invalid_frequency();
    }


  m_frequency                   =                                freq;
  m_number_of_samples_per_cycle = g_number_of_samples_per_second/freq;

    if(m_number_of_samples_per_cycle <= 0.0)
    {
      throw invalid_number_of_samples_per_cycle();
    }
}


void
sound_device::
set_absolute_fm(f32_t  target_freq, uint32_t  num_steps, uint32_t  ms)
{
  set_relative_fm(target_freq-m_frequency,num_steps,ms);
}


void
sound_device::
set_relative_fm(f32_t  freq, uint32_t  num_steps, uint32_t  ms)
{
    if(!num_steps)
    {
      throw invalid_number_of_steps();
    }


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


void
sound_device::
set_absolute_vm(sample_t  target_vol, uint32_t  num_steps, uint32_t  ms)
{
  set_relative_vm(target_vol-m_volume,num_steps,ms);
}


void
sound_device::
set_relative_vm(sample_t  vol, uint32_t  num_steps, uint32_t  ms)
{
    if(!num_steps)
    {
      throw invalid_number_of_steps();
    }


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




