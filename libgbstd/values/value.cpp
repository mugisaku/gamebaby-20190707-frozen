#include"libgbstd/value.hpp"




namespace gbstd{




value&
value::
assign(bool   b) noexcept
{
  clear();

  new(&m_data) bool(b);

  m_kind = kind::boolean;

  return *this;
}


value&
value::
assign(int  i) noexcept
{
  clear();

  new(&m_data) int(i);

  m_kind = kind::integer;

  return *this;
}


value&
value::
assign(const char*  s) noexcept
{
  clear();

  new(&m_data) std::string(s);

  m_kind = kind::string;

  return *this;
}


value&
value::
assign(user_input  kbd) noexcept
{
  clear();

  new(&m_data) user_input(kbd);

  m_kind = kind::user_input;

  return *this;
}


value&
value::
assign(variable&&  var) noexcept
{
  clear();

  m_data.var =  new variable(std::move(var));

  m_kind = kind::variable;

  return *this;
}


value&
value::
assign(value_array&&  arr) noexcept
{
  clear();

  m_data.arr = new value_array(std::move(arr));

  m_kind = kind::array;

  return *this;
}


value&
value::
assign(value_list&&  ls) noexcept
{
  clear();

  m_data.ls = new value_list(std::move(ls));

  m_kind = kind::list;

  return *this;
}


value&
value::
assign(const value&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::boolean): m_data.b = rhs.m_data.b;break;
      case(kind::integer): m_data.i = rhs.m_data.i;break;
      case(kind::string ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::user_input): m_data.ui = rhs.m_data.ui;break;
      case(kind::variable): m_data.var = new variable(*rhs.m_data.var);break;
      case(kind::array): m_data.arr = new value_array(*rhs.m_data.arr);break;
      case(kind::list): m_data.ls = new value_list(*rhs.m_data.ls);break;
        }
    }


  return *this;
}


value&
value::
assign(value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::boolean): m_data.b = rhs.m_data.b;break;
      case(kind::integer ): m_data.i = rhs.m_data.i;break;
      case(kind::string  ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::user_input): m_data.ui = rhs.m_data.ui;break;
      case(kind::variable): m_data.var = rhs.m_data.var;break;
      case(kind::array): m_data.arr = rhs.m_data.arr;break;
      case(kind::list): m_data.ls = rhs.m_data.ls;break;
        }
    }


  return *this;
}




void
value::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::boolean): break;
  case(kind::integer): break;
  case(kind::string ): gbstd::destruct(m_data.s);break;
  case(kind::variable): delete m_data.var;break;
  case(kind::array): delete m_data.arr;break;
  case(kind::list): delete m_data.ls;break;
    }


  m_kind = kind::null;
}


void
value::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null): printf("null");break;
  case(kind::boolean): printf("%s",m_data.b? "true":"false");break;
  case(kind::integer): printf("%d",m_data.i);break;
  case(kind::string ): printf("\"%s\"",m_data.s.data());break;
  case(kind::user_input): printf("user_input()");break;
  case(kind::variable): m_data.var->print();break;
  case(kind::list): m_data.ls->print();break;
  case(kind::array): m_data.arr->print();break;
  default: printf("unknown");break;
    }
}




}




