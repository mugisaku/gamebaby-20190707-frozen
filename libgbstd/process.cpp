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
      m_interval  = rhs.m_interval;

      m_execution = std::move(rhs.m_execution);
    }


  return *this;
}


process&
process::
assign(const char*  name, uint32_t  intval, execution_frame&&  frame) noexcept
{
  clear();

  m_name = name;

  m_execution.push(std::move(frame));

  return *this;
}




void
process::
clear() noexcept
{
  m_interval = 0;

  m_execution.clear();
}


void
process::
step() noexcept
{
    if(g_time >= m_next_time)
    {
        if(m_execution)
        {
          m_execution(8);
        }


      m_next_time = g_time+m_interval;
    }
}




}




