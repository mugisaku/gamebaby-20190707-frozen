#include"libgbstd/process.hpp"




namespace gbstd{




process&
process::
assign(uint32_t  interval, std::initializer_list<execution_entry>  ls) noexcept
{
    if(!interval)
    {
      printf("oops\n");

      interval = 20;
    }


  m_interval = interval;

  m_next_time = 0;

  m_main_stack.clear();
  m_buffer_stack.clear();

  m_main_stack.emplace_back(ls);

  return *this;
}




void
process::
merge() noexcept
{
    for(auto&&  frm: m_buffer_stack)
    {
      m_main_stack.emplace_back(std::move(frm));
    }


  m_buffer_stack.clear();
}


void
process::
step() noexcept
{
    while(g_time >= m_next_time)
    {
      int  counter = 8;

      merge();

        while(get_number_of_frames())
        {
          auto&  top = m_main_stack.back();

            if(top.m_pc < top.get_length())
            {
              auto&  ent = top[top.m_pc];

              ent.m_callback(*this,ent.m_data);

              auto  stop_sign = ent.m_stop_sign;

                if(m_pop_flag)
                {
                  m_main_stack.pop_back();

                  m_pop_flag = false;
                }


              merge();

                if(!--counter || stop_sign)
                {
                  goto END;
                }
            }

          else
            {
              m_main_stack.pop_back();

              m_pop_flag = false;
            }
        }


END:
        if(!m_interval)
        {
          m_interval = g_time;
        }

      else
        {
          m_next_time += m_interval;
        }
    }
}




}




