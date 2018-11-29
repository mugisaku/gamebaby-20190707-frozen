#include"libgbstd/job.hpp"




namespace gbstd{




job&
job::
assign(job&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_process = std::move(rhs.m_process);
    }


  return *this;
}


void
job::
clear() noexcept
{
  m_process.clear();
}


bool
job::
is_busy() const noexcept
{
  return m_process.get_foreground_job_list();
}


bool
job::
is_finished() const noexcept
{
  return m_process.is_exited();
}


void
job::
step() noexcept
{
  m_process.step();
}


}




