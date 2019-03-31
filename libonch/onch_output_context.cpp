#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




int
onch_output_context::
get_v_index(const onch_word&  w) noexcept
{
    if(w.test_v_index())
    {
      m_last_v_index = w.get_v_index();
    }


  return m_last_v_index;
}


int
onch_output_context::
get_f_index(const onch_word&  w) noexcept
{
    if(w.test_f_index())
    {
      m_last_f_index = w.get_f_index();
    }


  return m_last_f_index;
}


int
onch_output_context::
get_l_index(const onch_word&  w) noexcept
{
    if(w.test_l_index())
    {
      m_last_l_index = w.get_l_index();
    }


  return m_last_l_index;
}




}




