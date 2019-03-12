#include"libgbstd/sound.hpp"




namespace gbstd{




void
modulation_status::
clear() noexcept
{
  m_current   = 0;
  m_increment = 0;

  m_initial_number_of_remain_samples = 0;
  m_number_of_remain_samples         = 0;
  m_number_of_steps                  = 0;

  m_changed = false;
}


void
modulation_status::
step()
{
    if(m_number_of_steps)
    {
        if(!--m_number_of_remain_samples)
        {
          --m_number_of_steps;

          m_number_of_remain_samples = m_initial_number_of_remain_samples;

          change_current(m_current+m_increment);
        }
    }
}


void
modulation_status::
change_current(f32_t  v)
{
  m_current = v;

    if(m_current <= 0.0)
    {
      throw invalid();
    }


  m_changed = true;
}


void
modulation_status::
set(f32_t  start, f32_t  target, uint32_t  num_steps, uint32_t  ms)
{
  auto  diff = target-start;

    if(!num_steps)
    {
      throw invalid_number_of_steps();
    }


  m_current   =                    start;
  m_increment = std::abs(diff/num_steps);

    if(m_current > diff)
    {
      m_increment = -m_increment;
    }


  m_number_of_steps = num_steps;

  auto&  init_num_samples = m_initial_number_of_remain_samples;
  auto&       num_samples =         m_number_of_remain_samples;

                init_num_samples = g_number_of_samples_per_millisecond*ms/num_steps;
  num_samples = init_num_samples                                                   ;

  m_changed = true;
}




}




