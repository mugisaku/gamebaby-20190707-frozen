#include"libgbstd/weak_reference_counter.hpp"




namespace gbstd{




struct
weak_reference_counter::
data
{
  int  m_value;
  int  m_weak_value;

  data*  m_next;

};


weak_reference_counter::data*
weak_reference_counter::
m_dead_top=nullptr;




weak_reference_counter::
weak_reference_counter() noexcept
{
    if(!m_dead_top)
    {
      m_dead_top = new data;

      m_dead_top->m_next = nullptr;
    }


  m_data = m_dead_top                     ;
           m_dead_top = m_dead_top->m_next;

  m_data->m_value      = 0;
  m_data->m_weak_value = 1;
}




weak_reference_counter::
operator bool() const noexcept
{
  return m_data && m_data->m_value;
}


weak_reference_counter&
weak_reference_counter::
assign(const weak_reference_counter&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

      ++m_data->m_weak_value;
    }


  return *this;
}


weak_reference_counter&
weak_reference_counter::
assign(weak_reference_counter&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


void
weak_reference_counter::
unrefer() noexcept
{
    if(m_data)
    {
      auto&  n = m_data->m_weak_value;

        if(!--n)
        {
          m_data->m_next = m_dead_top         ;
                           m_dead_top = m_data;
        }


      m_data = nullptr;
    }
}


weak_reference_counter&
weak_reference_counter::
set_value(int  v) noexcept
{
  m_data->m_value = v;

  return *this;
}


const int&
weak_reference_counter::
get_value() const noexcept
{
  return m_data->m_value;
}


void
weak_reference_counter::
reset() noexcept
{
  unrefer();
}




template<typename  T>
int
count(T*  ptr) noexcept
{
  int  n = 0;

    while(ptr)
    {
      ++n;

      ptr = ptr->m_next;
    }


  return n;
}


void
weak_reference_counter::
print_dead() noexcept
{
  printf("weak refcount %d",count(m_dead_top));
}




}




