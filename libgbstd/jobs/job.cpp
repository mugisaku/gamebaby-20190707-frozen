#include"libgbstd/job.hpp"




namespace gbstd{




job&
job::
assign(const job&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_process = new process(*rhs.m_process);
    }


  return *this;
}


job&
job::
assign(job&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_process,rhs.m_process);
    }


  return *this;
}


job&
job::
assign(process*  proc) noexcept
{
  clear();

  m_process = proc;

  return *this;
}


job&
job::
assign(process_callback  cb, uint32_t  intval) noexcept
{
  return assign(new process(cb,intval));
}


void
job::
clear() noexcept
{
  delete m_process          ;
         m_process = nullptr;
}


bool
job::
is_finished() const noexcept
{
  return m_process->is_exited();
}


void
job::
step() noexcept
{
  m_process->step();
}


}




