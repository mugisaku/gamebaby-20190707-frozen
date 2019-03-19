#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




void
onch_definition::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null):
  case(kind::volume):
  case(kind::frequency):
  case(kind::length):
  case(kind::sound):
      break;
  case(kind::table):
      m_data.tbl.~onch_table();
      break;
    }


  m_kind = kind::null;
  m_name.clear();
}


onch_definition&
onch_definition::
assign(const std::string&  name, onch_volume      v) noexcept
{
  clear();

  m_kind = kind::volume;
  m_name = name;
  m_data.v = v;

  return *this;
}


onch_definition&
onch_definition::
assign(const std::string&  name, onch_frequency   f) noexcept
{
  clear();

  m_kind = kind::frequency;
  m_name = name;
  m_data.f = f;

  return *this;
}


onch_definition&
onch_definition::
assign(const std::string&  name, onch_length  l) noexcept
{
  clear();

  m_kind = kind::length;
  m_name = name;
  m_data.l = l;

  return *this;
}


onch_definition&
onch_definition::
assign(const std::string&  name, onch_sound  snd) noexcept
{
  clear();

  m_kind = kind::sound;
  m_name = name;
  m_data.snd = snd;

  return *this;
}


onch_definition&
onch_definition::
assign(const std::string&  name, onch_table&&  tbl) noexcept
{
  clear();

  m_kind = kind::table;
  m_name = name;
  new(&m_data) onch_table(std::move(tbl));

  return *this;
}


void
onch_definition::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null     ): printf("null");break;
  case(kind::volume   ): printf("volume{%f}",m_data.v.get_value());break;
  case(kind::frequency): printf("frequency{%f}",m_data.f.get_value());break;
  case(kind::length   ): printf("length{%u}",m_data.l.get_value());break;
  case(kind::sound    ): m_data.snd.print();break;
  case(kind::table    ): m_data.tbl.print();break;
    }
}




}




