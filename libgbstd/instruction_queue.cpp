#include"libgbstd/instruction_queue.hpp"




namespace gbstd{




void
instruction_queue::
clear() noexcept
{
  m_container = {0};

  m_rp = 0;
  m_wp = 0;
}


void
instruction_queue::
put(intptr_t  v) noexcept
{
    if(m_wp == m_container.size())
    {
      m_container.emplace_back(v);
    }

  else
    {
      m_container[m_wp] = v;
    }


  ++m_wp;
}


void
instruction_queue::
invoke(void*  data) noexcept
{
  auto  n = m_container[m_rp];

  ++m_rp;

    while(n--)
    {
      auto  callback = reinterpret_cast<void(*)(dummy&,int)>(m_container[m_rp++]);
      auto  opvalue  = static_cast<int>(m_container[m_rp++]);

        if(callback)
        {
          callback(*static_cast<dummy*>(data),opvalue);
        }
    }
}


void
instruction_queue::
pop() noexcept
{
  auto  n = m_container[m_rp];

  m_rp += 1+(2*n);
}


void
instruction_queue::
push(std::initializer_list<instruction>  ls) noexcept
{
    if(!*this)
    {
      clear();
    }


  put(ls.size());

    for(auto  instr: ls)
    {
      put(instr.callback);
      put(instr.opvalue );
    }


  put(0);

  --m_wp;
}




}




