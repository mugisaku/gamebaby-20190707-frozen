#include"libgbstd/job.hpp"




namespace gbstd{




struct
job_list::
element
{
  job  m_job;

  element*  m_next;

};



job&
job_list::iterator::
operator*() const noexcept
{
  return m_data->m_job;
}


job_list::iterator&
job_list::iterator::
operator++() noexcept
{
  m_data = m_data->m_next;

  return *this;
}


job_list::iterator
job_list::iterator::
operator++(int) noexcept
{
  auto  tmp = *this;

  m_data = m_data->m_next;

  return tmp;
}




void
job_list::
clear() noexcept
{
  auto  next = m_first;

    while(next)
    {
      auto  now = next        ;
                  next->m_next;

      delete now;
    }


  m_first = nullptr;

  m_number_of_jobs = 0;
}


void
job_list::
add(job&&  job) noexcept
{
  auto  e = new element;

  e->m_job = std::move(job);

  e->m_next = m_first    ;
              m_first = e;

  ++m_number_of_jobs;
}


void
job_list::
step() noexcept
{
START:
    if(m_first)
    {
        if(m_first->m_job.is_finished())
        {
          auto  next = m_first->m_next;

          delete m_first       ;
                 m_first = next;

          --m_number_of_jobs;

          goto START;
        }

      else
        {
          m_first->m_job.step();
        }
    }
}


void
job_list::
step_all() noexcept
{
  element*  last = nullptr;
  element*  next = m_first;

    while(next)
    {
      auto  now = next              ;
                  next = now->m_next;

        if(now->m_job.is_finished())
        {
          delete now;

          --m_number_of_jobs;

            if(last)
            {
              last->m_next = next;
            }

          else
            {
              m_first = nullptr;

              return;
            }
        }

      else
        {
          now->m_job.step();

          last = now;
        }
    }
}




}




