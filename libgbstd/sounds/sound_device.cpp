#include"libgbstd/sound.hpp"




namespace gbstd{




void
sound_device::
input(const sound_event&  evt) noexcept
{
  auto  l = evt.get_length();

  m_vm_status.set(   evt.get_start_volume(),   evt.get_end_volume(),evt.get_number_of_vm_steps(),l);
  m_fm_status.set(evt.get_start_frequency(),evt.get_end_frequency(),evt.get_number_of_fm_steps(),l);

  m_number_of_remain_samples_for_timer = get_number_of_samples_by_time(l);

  wake();
}


void
sound_device::
reset() noexcept
{
  m_status.clear();

  m_fm_status.clear();
  m_vm_status.clear();

  m_event_queue.clear();

  m_event_index = 0;
}




void
sound_device::
process_timer() noexcept
{
    if(m_number_of_remain_samples_for_timer)
    {
        if(!--m_number_of_remain_samples_for_timer)
        {
          pump_event();
        }
    }
}


void
sound_device::
pump_event() noexcept
{
    if(m_event_index < m_event_queue.size())
    {
      input(m_event_queue[m_event_index++]);
    }

  else
    {
      sleep();
    }
}


void
sound_device::
check_frequency()
{
    if(m_fm_status.m_changed)
    {
      m_fm_status.m_changed = false;

      on_frequency_changed();
    }
}


void
sound_device::
put(sample_t  src, sample_t&  dst)
{
  process_timer();

  m_vm_status.step();
  m_fm_status.step();

  check_frequency();

    if(!is_slept() || !is_muted())
    {
      dst += src;
    }
}




void
sound_device::
push(std::initializer_list<sound_event>  ls)
{
  m_event_queue = ls;

  m_event_index = 0;

  m_event_queue = ls;

  pump_event();
}




void
sound_device::
print() const noexcept
{
  printf("volume: %f\n",m_vm_status.m_current);
  printf("frequency: %f\n",m_fm_status.m_current);
  printf("num remain samples for timer: %d\n",m_number_of_remain_samples_for_timer);
}




}




