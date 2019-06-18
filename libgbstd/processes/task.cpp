#include"libgbstd/process.hpp"




namespace gbstd{




task&
task::
clear() noexcept
{
  clear_status();

  m_name.clear();
  m_id.clear();

  m_data = nullptr;

  m_draw_callback   = nullptr;
  m_tick_callback   = nullptr;
  m_finish_callback = nullptr;

  m_interval  = 0;
  m_next_time = 0;

  return *this;
}


task&
task::
die() noexcept
{
  m_status.unset(flags::live);

    if(m_status.test(flags::discard_when_die))
    {
      discard();
    }


  return *this;
}


task&
task::
getup() noexcept
{
  m_status.unset(flags::sleep);
  m_status.unset(flags::alarm);

    if(m_status.test(flags::die_when_getup))
    {
      die();
    }

  else
    {
      m_next_time = m_clock_watch.get_time()+m_interval;
    }


  return *this;
}


task&
task::
sleep() noexcept
{
  m_status.set(  flags::sleep);
  m_status.unset(flags::alarm);

  return *this;
}


task&
task::
sleep(uint32_t  t) noexcept
{
  m_status.set(flags::sleep);
  m_status.set(flags::alarm);

  m_next_time = m_clock_watch.get_time()+t;

  return *this;
}


task&
task::
discard_when_die() noexcept
{
  m_status.set(flags::discard_when_die);

  return *this;
}


task&
task::
set_blinking_rate(int  show, int  hide) noexcept
{
  m_blink_show_value = show;
  m_blink_hide_value = hide;

  return *this;
}


task&
task::
operator()() noexcept
{
  auto  now = m_clock_watch.get_time();

    if(is_living())
    {
        if(is_sleeping())
        {
            if(m_status.test(flags::alarm) && (now >= m_next_time))
            {
              getup();
            }
        }
    }


    while(is_living() && !is_sleeping() && (now >= m_next_time) && m_tick_callback)
    {
      m_tick_callback(*static_cast<dummy*>(m_data));

        if(!m_interval)
        {
          break;
        }


      m_next_time += m_interval;
    }


  return *this;
}


task&
task::
operator()(const canvas&  cv) noexcept
{
    if(is_living())
    {
        if(is_blinking())
        {
            if(m_blink_counter)
            {
              --m_blink_counter;
            }

          else
            {
              m_blink_counter = m_status.test(flags::blink_bit)? m_blink_show_value
                               :                                 m_blink_hide_value;

              m_status.reverse(flags::blink_bit);
            }
        }


        if(is_showing() && m_draw_callback)
        {
            if(!is_blinking() || m_status.test(flags::blink_bit))
            {
              m_draw_callback(cv,*reinterpret_cast<dummy*>(m_data));
            }
        }
    }


  return *this;
}


task&
task::
finish() noexcept
{
    if(m_finish_callback)
    {
      m_finish_callback(*static_cast<dummy*>(m_data));
    }


  clear();

  return *this;
}




}




