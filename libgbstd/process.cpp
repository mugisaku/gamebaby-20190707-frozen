#include"libgbstd/process.hpp"




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
      m_base_callback = rhs.m_base_callback;
      m_data = rhs.m_data;

      m_execution = std::move(rhs.m_execution);
    }


  return *this;
}


process&
process::
assign(const char*  name, uint32_t  intval, base_callback  cb, void*  dat) noexcept
{
  clear();

  m_name = name;

  m_base_callback = cb;

  m_data = dat;

  return *this;
}




void
process::
clear() noexcept
{
  m_name.clear();

  m_interval = 0;

  m_base_callback = nullptr;

  m_execution.clear();
}


void
process::
step() noexcept
{
    if(g_time >= m_next_time)
    {
      int  n = 8;

BEGIN:
        if(!m_execution)
        {
          m_base_callback(m_execution,m_data);
        }


      auto  last = m_execution.get();

        if(last)
        {
            for(;;)
            {
              last(m_execution,m_data);

              auto  now = m_execution.get();

                if(!now)
                {
                    if(!--n)
                    {
                      break;
                    }


                  goto BEGIN;
                }

              else
                if(now == last)
                {
                  break;
                }


              last = now;
            }
        }


      m_next_time = g_time+m_interval;
    }
}




}




