#include"libgbstd/value.hpp"




namespace gbstd{




value_array&
value_array::
assign(const value_array&   rhs) noexcept
{
    if(this != &rhs)
    {
      resize(rhs.size());

      auto  dst     =     begin();
      auto  src     = rhs.begin();
      auto  src_end = rhs.end();

        while(src != src_end)
        {
          *dst++ = *src++;
        }
    }


  return *this;
}


value_array&
value_array::
assign(value_array&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_data  ,rhs.m_data  );
      std::swap(m_number_of_elements,rhs.m_number_of_elements);
      std::swap(m_length,rhs.m_length);
    }


  return *this;
}


void
value_array::
clear() noexcept
{
  delete[] m_data          ;
           m_data = nullptr;

  m_number_of_elements = 0;

  m_length = 0;
}


void
value_array::
reallocate(size_t  n) noexcept
{
    if(m_number_of_elements < n)
    {
      auto  old_data = m_data               ;
                       m_data = new value[n];

        for(int  i = 0;  i < m_length;  ++i)
        {
          m_data[i] = std::move(old_data[i]);
        }


      delete[] old_data;
    }


  m_number_of_elements = n;
}


void
value_array::
resize(size_t  n) noexcept
{
  reallocate(n);

  m_length = n;
}


value&
value_array::
push(value  v) noexcept
{
    if(m_length >= m_number_of_elements)
    {
      auto   n = m_number_of_elements;

      reallocate((n < 256)? (n+2)*2:n*2);
    }


  auto&  dst = m_data[m_length++];

  dst = std::move(v);

  return dst;
}


value&
value_array::
pop() noexcept
{
  return m_data[--m_length];
}




variable*
value_array::
find_variable(const char*  name) const noexcept
{
    for(auto&  v: *this)
    {
        if(v.is_variable())
        {
          auto&  var = v.get_variable();

            if(var.get_name() == name)
            {
              return &var;
            }
        }
    }


  return nullptr;
}




void
value_array::
print() const noexcept
{
  printf("[");

    for(auto&  v: *this)
    {
      v.print();

      printf(", ");
    }


  printf("]");
}




}




