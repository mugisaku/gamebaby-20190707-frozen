#include"libgbstd/clock.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{




struct
clock_entity
{
  std::string  m_name;

  uint32_t  m_time;
  uint32_t  m_buffer;

  int  m_permil;

  bool  m_working_flag;

  clock_entity*  m_next;

};




const uint32_t&  clock_control::get_time()   const noexcept{return m_entity->m_time;}
const int&       clock_control::get_permil() const noexcept{return m_entity->m_permil;}

bool  clock_control::is_working() const noexcept{return  m_entity->m_working_flag;}
bool  clock_control::is_stopped() const noexcept{return !m_entity->m_working_flag;}


void
clock_control::
reset() noexcept
{
  m_entity->m_time   = 0;
  m_entity->m_buffer = 0;
}


clock_control&  clock_control::set_time(uint32_t  v) noexcept{  m_entity->m_time   = v;  return *this;}
clock_control&  clock_control::set_permil(   int  v) noexcept{  m_entity->m_permil = v;  return *this;}

void    clock_control::start() noexcept{            m_entity->m_working_flag =  true;}
void  clock_control::restart() noexcept{  reset();  m_entity->m_working_flag =  true;}
void     clock_control::stop() noexcept{            m_entity->m_working_flag = false;}




const uint32_t&  clock_watch::get_time()   const noexcept{return m_entity->m_time;}
const int&       clock_watch::get_permil() const noexcept{return m_entity->m_permil;}

bool  clock_watch::is_working() const noexcept{return  m_entity->m_working_flag;}
bool  clock_watch::is_stopped() const noexcept{return !m_entity->m_working_flag;}




clock_master::
~clock_master()
{
  auto  ptr = m_top_pointer;

    while(ptr)
    {
      auto  next = ptr->m_next;

      delete ptr       ;
             ptr = next;
    }
}


clock_control
clock_master::
operator[](std::string_view  name) noexcept
{
  auto  ptr = m_top_pointer;

    while(ptr)
    {
        if(ptr->m_name == name)
        {
          return clock_control(*ptr);
        }


      ptr = ptr->m_next;
    }


  return add(name);
}




clock_control
clock_master::
add(std::string_view  name, int  permil) noexcept
{
  auto  ent = new clock_entity;

  ent->m_name         =   name;
  ent->m_permil       = permil;
  ent->m_working_flag =  false;
  ent->m_time         =      0;
  ent->m_buffer       =      0;

  ent->m_next = m_top_pointer      ;
                m_top_pointer = ent;


  return clock_control(*ent);
}


void
clock_master::
step() noexcept
{
  auto  diff = g_time-m_last_time         ;
                      m_last_time = g_time;

  auto  ptr = m_top_pointer;

    while(ptr)
    {
        if(ptr->m_working_flag)
        {
          constexpr int  shift_amount = 16;

          ptr->m_buffer += (diff<<shift_amount)/1000*ptr->m_permil;

          ptr->m_time += (ptr->m_buffer>>shift_amount);

          ptr->m_buffer &= 0xFFFF;
        }


      ptr = ptr->m_next;
    }
}


void
clock_master::
print() const noexcept
{
  auto  ptr = m_top_pointer;

    while(ptr)
    {
      printf("{\"%s\": %8u}",ptr->m_name.data(),ptr->m_time);

      ptr = ptr->m_next;
    }
}


clock_watch
clock_master::
get_zero_clock_watch() noexcept
{
  static const clock_entity  zero = {"!zero",0,0,0};

  return clock_watch(zero);
}




}




