#include"libgbstd/job.hpp"




namespace gbstd{




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




