#include"libgbstd/clock.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{




const std::string&  clock_watch::get_name()  const noexcept{return  m_clock->get_name();}
const uint32_t&     clock_watch::get_value() const noexcept{return m_clock->get_value();}




void
clock::
reset() noexcept
{
  m_value  = 0;
  m_buffer = 0;
}


void
clock::
step(uint32_t  t) noexcept
{
    if(!m_stopped_flag)
    {
      constexpr int  shift_amount = 16;

      m_buffer += (t<<shift_amount)/1000*m_permil;

      m_value += (m_buffer>>shift_amount);

      m_buffer &= 0xFFFF;
    }
}


void
clock::
print() const noexcept
{
  printf("{\"%s\": %8u}",m_name.data(),m_value);
}




clock&
clock_master::
operator[](const std::string&  name) noexcept
{
    for(auto&  tm: m_clock_list)
    {
        if(tm.get_name() == name)
        {
          return tm;
        }
    }


  static clock  zero_timer("zero",0);

  return zero_timer;
}


clock*
clock_master::
find(const std::string&  name) noexcept
{
    for(auto&  tm: m_clock_list)
    {
        if(tm.get_name() == name)
        {
          return &tm;
        }
    }


  return nullptr;
}


void
clock_master::
step() noexcept
{
  auto  diff = g_time-m_last_time         ;
                      m_last_time = g_time;

    for(auto&  tm: m_clock_list)
    {
      tm.step(diff);
    }
}


void
clock_master::
print() const noexcept
{
    for(auto&  tm: m_clock_list)
    {
      tm.print();
    }
}




}




