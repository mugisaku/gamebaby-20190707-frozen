#include"libgbstd/value.hpp"




namespace gbstd{




variable&
variable::
assign(const char*  name, const value&   v) noexcept
{
  m_name = name;
  m_value = v;

  return *this;
}


variable&
variable::
assign(const char*  name, value&&  v) noexcept
{
  m_name = name;
  m_value = std::move(v);

  return *this;
}


void
variable::
print() const noexcept
{
  printf("%s: ",m_name.data());

  m_value.print();
}


}




