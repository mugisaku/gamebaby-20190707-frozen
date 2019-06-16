#include"libgbstd/process.hpp"




namespace gbstd{




task&
task::
sleep(uint32_t  t) noexcept
{
  m_status.set(flags::sleep);
  m_status.set(flags::alarm);

  m_next_time = g_time*t;

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
    if(is_living())
    {
      auto  now = m_clock_watch.get_time();

        if(is_sleeping())
        {
            if(m_status.test(flags::alarm) && (now >= m_next_time))
            {
              m_status.unset(flags::alarm);
            }

          else
            {
              return *this;
            }
        }


        while((now >= m_next_time) && m_tick_callback && !is_freezing())
        {
          m_tick_callback(*static_cast<dummy*>(m_data));

            if(!m_interval)
            {
              break;
            }


          m_next_time += m_interval;
        }
    }


  return *this;
}


task&
task::
operator()(const canvas&  cv) noexcept
{
    if(is_living() && !is_sleeping())
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




}




