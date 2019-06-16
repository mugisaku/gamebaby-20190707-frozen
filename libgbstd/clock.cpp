#include"libgbstd/clock.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{




clock&
clock::
reset(uint32_t  time, uint32_t  fraction) noexcept
{
  m_time     =     time;
  m_fraction = fraction;

  m_status.clear();

  return *this;
}


clock&
clock::
add(uint32_t  t) noexcept
{
    if(m_working_flag)
    {
      constexpr int  shift_amount = 16;

      m_fraction += (t<<shift_amount)/1000*m_permil;

      m_time += (m_fraction>>shift_amount);

      m_fraction &= 0xFFFF;
    }


  return *this;
}


const clock
clock_watch::
m_null("null");




clock_master&
clock_master::
add(clock&  cl) noexcept
{
  m_list.emplace_back(&cl);

  return *this;
}


clock_master&
clock_master::
remove(std::string_view  name) noexcept
{
  auto  it = m_list.begin();

    while(it != m_list.end())
    {
        if((*it)->m_name == name)
        {
          it = m_list.erase(it);
        }

      else
        {
          ++it;
        }
    }


  return *this;
}


clock_master&
clock_master::
update() noexcept
{
  auto  diff = g_time-m_last_time         ;
                      m_last_time = g_time;

    for(auto&  ptr: m_list)
    {
      ptr->add(diff);
    }


  return *this;
}


void
clock_master::
print() const noexcept
{
    for(auto&  ptr: m_list)
    {
      printf("{\"%s\": %8u}",ptr->get_name().data(),ptr->get_time());
    }
}




}




