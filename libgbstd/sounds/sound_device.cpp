#include"libgbstd/sound.hpp"




namespace gbstd{




namespace{
void
set_modulation(f32_t  start, f32_t  end, uint32_t  ms, f32_t&  cur, f32_t&  inc) noexcept
{
    if(ms <= 1)
    {
      cur = end;
      inc =   0;

      return;
    }


  auto  diff = std::abs(end-start);

  cur = start;
  inc = diff/ms;

    if(start > end)
    {
      inc = -inc;
    }
}
}




sound_device::
sound_device(uint32_t  sampling_rate, const sound_instruction&  instr) noexcept
{
  set_sampling_rate(sampling_rate);

  reset(instr);
}




void
sound_device::
reset(const sound_instruction&  instr) noexcept
{
  m_length = instr.get_length();

  set_modulation(instr.get_start_volume()   ,instr.get_end_volume()   ,m_length,m_vm_current,m_vm_increment);
  set_modulation(instr.get_start_frequency(),instr.get_end_frequency(),m_length,m_fm_current,m_fm_increment);

  update_number_of_samples();
}


void
sound_device::
set_sampling_rate(uint32_t  rate) noexcept
{
  m_sampling_rate = rate;

  update_number_of_samples();
}


void
sound_device::
mix(f32_t*  ptr) noexcept
{
  update(get_sampling_rate()/get_frequency());

  m_number_of_remain_samples = m_number_of_upward_samples;


  auto  n = get_number_of_samples();

  uint32_t  initial_num_permil = get_sampling_rate()/1000;
  uint32_t          num_permil = initial_num_permil;

    while(n--)
    {
        if(!num_permil)
        {
          num_permil = initial_num_permil;

          m_vm_current += m_vm_increment;
          m_fm_current += m_fm_increment;
        }

      else
        {
          --num_permil;
        }


      uint32_t  safe_counter = 0;

        while(m_number_of_remain_samples < 1)
        {
            if(++safe_counter > 0x1000000)
            {
              report;
               break;
            }


            if(is_downward())
            {
              update(get_sampling_rate()/get_frequency());

              m_number_of_remain_samples += m_number_of_upward_samples;
            }

          else
            {
              m_number_of_remain_samples += m_number_of_downward_samples;
            }


          m_downward_flag = !m_downward_flag;
        }


      *ptr++ = get_sample();

      m_number_of_remain_samples -= 1;
    }
}




}




