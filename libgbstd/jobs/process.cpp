#include"libgbstd/job.hpp"




namespace gbstd{




process&
process::
assign(process&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_name = std::move(rhs.m_name);
      m_next_time = rhs.m_next_time;

      m_interval = rhs.m_interval;
      m_callback = rhs.m_callback;

      std::swap(m_data,rhs.m_data);

      m_pc = rhs.m_pc;

      m_state = rhs.m_state;
    }


  return *this;
}


process&
process::
assign(const char*  name, process_callback  cb, uint32_t  intval, void*  dat) noexcept
{
  clear();

  m_name     = name;
  m_callback = cb;
  m_interval = intval;
  m_data     = dat;

  return *this;
}




void
process::
clear() noexcept
{
  reset();

  m_foreground_job_list.clear();
  m_background_job_list.clear();

  m_name.clear();
  m_interval = 0;
  m_callback = nullptr;
  m_data = nullptr;
}


void
process::
reset() noexcept
{
  m_state = state::running;

  m_next_time = 0;

  m_pc = 0;
}


void
process::
execute() noexcept
{
    if(m_callback && (gbstd::g_time >= m_next_time))
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
        if(m_foreground_job_list)
        {
          m_foreground_job_list.step();
        }


        if(!m_foreground_job_list)
        {
          execute();
        }


      m_background_job_list.step();
    }

  else
    if(is_waiting())
    {
      m_background_job_list.step_all();

        if(!m_background_job_list)
        {
          run();

          goto START;
        }
    }
}




}




