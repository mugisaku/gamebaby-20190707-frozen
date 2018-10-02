#include"libgbstd/value.hpp"




namespace gbstd{




value_list&
value_list::
assign(std::initializer_list<value>  ls) noexcept
{
  clear();

    for(auto&&  v: ls)
    {
      push_back(std::move(v));
    }


  return *this;
}


value_list&
value_list::
assign(const value_list&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

        for(auto&  v: rhs)
        {
          push_back(v);
        }
    }


  return *this;
}


value_list&
value_list::
assign(value_list&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_first ,rhs.m_first );
      std::swap(m_last  ,rhs.m_last  );
      std::swap(m_length,rhs.m_length);
    }


  return *this;
}


void
value_list::
clear() noexcept
{
  auto  next = m_first;

    while(next)
    {
      auto  nd = next;
                 next = next->m_next;

      delete nd;
    }


  m_length = 0;
}


void
value_list::
resize(size_t  n) noexcept
{
    if(n < m_length)
    {
        while(n > m_length)
        {
          pop_back();
        }
    }

  else
    if(n > m_length)
    {
        while(n < m_length)
        {
          push_back(value());
        }
    }
}




value&
value_list::
push_front(value&&  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_prev = nullptr;

    if(m_first)
    {
      m_first->m_prev = nd;
    }

  else
    {
      m_last = nd;
    }


  nd->m_next = m_first     ;
               m_first = nd;

  ++m_length;


  return nd->m_value;
}


value&
value_list::
push_back(value&&  v)  noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_next = nullptr;

    if(m_last)
    {
      m_last->m_next = nd;
    }

  else
    {
      m_first = nd;
    }


  nd->m_prev = m_last     ;
               m_last = nd;

  ++m_length;


  return nd->m_value;
}


value
value_list::
pop_back()  noexcept
{
    if(m_last)
    {
      auto  v = std::move(m_last->m_value);

      auto  new_last = m_last->m_prev;

      delete m_last;
             m_last = new_last;

      new_last->m_next = nullptr;

        if(!m_length--)
        {
          m_first = nullptr;
        }



      return std::move(v);
    }


  return value();
}


value
value_list::
pop_front() noexcept
{
    if(m_first)
    {
      auto  v = std::move(m_first->m_value);

      auto  new_first = m_first->m_next;

      delete m_first            ;
             m_first = new_first;

      new_first->m_prev = nullptr;

        if(!m_length--)
        {
          m_last = nullptr;
        }


      return std::move(v);
    }


  return value();
}




value&
value_list::
insert_before(iterator  it, value  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_prev = nullptr;

  nd->m_next = it.m_node             ;
               it.m_node->m_prev = nd;

  ++m_length;


  return nd->m_value;
}


value&
value_list::
insert_after(iterator  it, value  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_next = nullptr;

  nd->m_prev = it.m_node             ;
               it.m_node->m_next = nd;

  ++m_length;


  return nd->m_value;
}


value_list::iterator
value_list::
erase(iterator  it) noexcept
{
    if(it)
    {
      auto  next = it.m_node->m_next;

      delete it.m_node;

      return iterator(next);
    }


  return iterator();
}


variable*
value_list::
find_variable(const char*  name) const noexcept
{
  std::string  name_s(name);

    for(auto&  v: *this)
    {
        if(v.is_variable())
        {
          auto&  var = v.get_variable();

            if(var.get_name() == name_s)
            {
              return &var;
            }
        }
    }


  return nullptr;
}




void
value_list::
print() const noexcept
{
  printf("{");

    for(auto&  v: *this)
    {
      v.print();

      printf(", ");
    }


  printf("}");
}




}




