#include"libgbstd/parser.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




token&
token::
assign(const token&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_info = rhs.m_info;
      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::floating_point_number):
          m_data.f = rhs.m_data.f;
          break;
      case(kind::operator_code):
          m_data.opco = rhs.m_data.opco;
          break;
      case(kind::binary_number):
      case(kind::octal_number):
      case(kind::decimal_number):
      case(kind::hexadecimal_number):
          m_data.n = rhs.m_data.n;
          break;
      case(kind::identifier):
      case(kind::single_quoted):
      case(kind::double_quoted):
          new(&m_data) std::string(rhs.m_data.s);
          break;
      case(kind::block):
          new(&m_data) token_block(rhs.m_data.blk);
          break;
        }
    }


  return *this;
}


token&
token::
assign(token&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_info,rhs.m_info);
      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::floating_point_number):
          m_data.f = rhs.m_data.f;
          break;
      case(kind::operator_code):
          m_data.opco = rhs.m_data.opco;
          break;
      case(kind::binary_number):
      case(kind::octal_number):
      case(kind::decimal_number):
      case(kind::hexadecimal_number):
          m_data.n = rhs.m_data.n;
          break;
      case(kind::identifier):
      case(kind::single_quoted):
      case(kind::double_quoted):
          new(&m_data) std::string(std::move(rhs.m_data.s));
          break;
      case(kind::block):
          new(&m_data) token_block(std::move(rhs.m_data.blk));
          break;
        }
    }


  return *this;
}


token&
token::
assign(token_info  info, uint64_t  n, int  sym) noexcept
{
  clear();

  m_info = info;

  m_data.n = n;

    switch(sym)
    {
  case('b'): m_kind = kind::binary_number     ;break;
  case('o'): m_kind = kind::octal_number      ;break;
  case('d'): m_kind = kind::decimal_number    ;break;
  case('x'): m_kind = kind::hexadecimal_number;break;
    }


  return *this;
}


token&
token::
assign(token_info  info, std::string&&  s, int  sym) noexcept
{
  clear();

  m_info = info;

  new(&m_data) std::string(std::move(s));

    switch(sym)
    {
  case('\''): m_kind = kind::single_quoted;break;
  case('\"'): m_kind = kind::double_quoted;break;
  case('\0'): m_kind = kind::identifier   ;break;
    }


  return *this;
}


token&
token::
assign(token_info  info, double  f) noexcept
{
  clear();

  m_info = info;

  m_data.f = f;

  m_kind = kind::floating_point_number;


  return *this;
}


token&
token::
assign(token_info  info, operator_code  opco) noexcept
{
  clear();

  m_info = info;

  m_data.opco = opco;

  m_kind = kind::operator_code;


  return *this;
}


token&
token::
assign(token_info  info, token_block&&  blk) noexcept
{
  clear();

  m_info = info;

  new(&m_data) token_block(std::move(blk));

  m_kind = kind::block;


  return *this;
}




void
token::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null):
  case(kind::binary_number):
  case(kind::octal_number):
  case(kind::decimal_number):
  case(kind::hexadecimal_number):
  case(kind::floating_point_number):
  case(kind::operator_code):
      break;
  case(kind::identifier):
  case(kind::single_quoted):
  case(kind::double_quoted):
      m_data.s.~basic_string();
      break;
  case(kind::block):
      m_data.blk.~token_block();
      break;
    }


  m_info = token_info();
  m_kind = kind::null;
}




void
token::
print_string(const std::string&  s, char  punct) noexcept
{
  printf("%c",punct);

    for(auto  c: s)
    {
           if(c == '\0'){printf("\\0");}
      else if(c == '\n'){printf("\\n");}
      else if(c == '\t'){printf("\\t");}
      else if(c == '\r'){printf("\\r");}
      else if(c == '\b'){printf("\\b");}
      else if(c == '\a'){printf("\\a");}
      else if(c == '\v'){printf("\\v");}
      else if(c == '\f'){printf("\\f");}
      else if(c == '\?'){printf("\\?");}
      else if(c == '\\'){printf("\\\\");}
      else if(c == '\''){printf("\\\'");}
      else if(c == '\"'){printf("\\\"");}
      else if(isascii(c)){printf("%c",c);}
      else               {printf("\\x%x",c);}
    }


  printf("%c",punct);
}


void
token::
print() const noexcept
{
  printf("[%4d] ",m_info.get_line_number()+1);

    switch(m_kind)
    {
  case(kind::null): ;break;
  case(kind::binary_number): printf("0b%llu",m_data.n);break;
  case(kind::octal_number): printf("0o%llu",m_data.n);break;
  case(kind::decimal_number): printf("%llu",m_data.n);break;
  case(kind::hexadecimal_number): printf("0x%llX",m_data.n);break;
  case(kind::identifier): printf("%s ",m_data.s.data());break;
  case(kind::single_quoted): print_string(m_data.s,'\'');break;
  case(kind::double_quoted): print_string(m_data.s,'\"');break;
  case(kind::floating_point_number): printf("%f",m_data.f);break;
  case(kind::operator_code): printf("%s",m_data.opco.get_string());break;
  case(kind::block): m_data.blk.print();break;
    }
}




}




