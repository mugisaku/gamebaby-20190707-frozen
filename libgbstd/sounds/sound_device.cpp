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

  m_vibrato_frequency = instr.get_vibrato_frequency();

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

  uint32_t  initial_vibr_counter = get_sampling_rate()/m_vibrato_frequency/2;
  uint32_t          vibr_counter = initial_vibr_counter;

  constexpr double  add_amount = 0.3;

  double  vibr_max_rate = 1.0+add_amount;
  double  vibr_min_rate = 1.0-add_amount;
  double  vibr_cur_rate =  vibr_min_rate;

  double  slice = (add_amount*2)/initial_vibr_counter;

  int  vibrato_state = 0;

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


        if(!vibr_counter)
        {
          vibr_counter = initial_vibr_counter;

          vibrato_state = !vibrato_state;
        }

      else
        {
          --vibr_counter;

          vibr_cur_rate += !vibrato_state? slice:-slice;
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
/*
              auto  rate = vibrato_state? 1.0+intensity
                          :               1.0-intensity;
*/

              update(get_sampling_rate()/(get_frequency()/*vibr_cur_rate*/));

              m_number_of_remain_samples += m_number_of_upward_samples;
            }

          else
            {
              m_number_of_remain_samples += m_number_of_downward_samples;
            }


          m_downward_flag = !m_downward_flag;
        }


      *ptr++ += get_sample();

      m_number_of_remain_samples -= 1;
    }
}




}




