#include"libgbstd/job.hpp"




namespace gbstd{




process::
process(process_callback  cb, uint32_t  intval, void*  dat) noexcept:
m_callback(cb),
m_interval(intval),
m_data(dat)
{
}




process&
process::
assign(const process&  rhs) noexcept
{
    if(this != &rhs)
    {
    }


  return *this;
}



process&
process::
assign(process&&  rhs) noexcept
{
    if(this != &rhs)
    {
    }


  return *this;
}




void
process::
execute() noexcept
{
    if(gbstd::g_time >= m_next_time)
    {
      m_callback(*this,m_data);

        if(m_interval)
        {
          m_next_time = gbstd::g_time+m_interval;
        }

      else
        {
          m_state = state::exited;
        }
    }
}


void
process::
step()noexcept
{
START:
    if(is_running())
    {
        if(m_foreground_job)
        {
            if(m_foreground_job.is_finished())
            {
              m_foreground_job.clear();

              execute();
            }

          else
            {
              m_foreground_job.step();
            }
        }

      else
        {
          execute();
        }


      m_background_job_list.step();
    }

  else
    if(is_waiting())
    {
      m_background_job_list.step();

        if(!m_background_job_list)
        {
          run();

          goto START;
        }
    }
}




}




