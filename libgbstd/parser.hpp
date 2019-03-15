#ifndef gbstd_parser_HPP
#define gbstd_parser_HPP


#include<cstddef>
#include<cstdint>
#include<string>
#include<vector>
#include"libgbstd/utility.hpp"


namespace gbstd{


class
operator_code
{
  char  m_data[4];

  static constexpr size_t  len(const char*  s) noexcept
  {
    return !s[0]? 0
          :!s[1]? 1
          :!s[2]? 2
          :       3;
          
  }

  static constexpr char  chr(const char*  s, int  i) noexcept
  {
    return (i < len(s))? s[i]:0;
  }

public:
  constexpr operator_code() noexcept: m_data{0,0,0,0}{}
  constexpr operator_code(const char*  s) noexcept: m_data{chr(s,0),chr(s,1),chr(s,2),0}{}

  constexpr const char*  get_string() const noexcept{return m_data;}
  constexpr int  get_length() const noexcept{return len(m_data);}

  constexpr operator uint32_t() const noexcept{return (m_data[0]<<24)|(m_data[1]<<16)|(m_data[2]<<8);}

  constexpr bool  operator==(operator_code  rhs) const noexcept
  {
    return (m_data[0] == rhs.m_data[0]) &&
           (m_data[1] == rhs.m_data[1]) &&
           (m_data[2] == rhs.m_data[2]);
  }

  constexpr bool  operator!=(operator_code  rhs) const noexcept
  {
    return !(*this == rhs);
  }

};


class token;
class tokenizer;


class
token_info
{
  const char*  m_pointer;

  int  m_line_number;

public:
  constexpr token_info(const char*  p=nullptr, int  ln=0) noexcept: m_pointer(p), m_line_number(ln){}

  constexpr const char*  get_pointer()     const noexcept{return m_pointer;}
  constexpr int          get_line_number() const noexcept{return m_line_number;}

};


class
token_block
{
  std::vector<token>  m_container;

  operator_code  m_open;
  operator_code  m_close;

public:
   token_block() noexcept{}
   token_block(std::vector<token>&&  c, operator_code  open, operator_code  close);
  ~token_block(){clear();}

  void  clear() noexcept;

  void  print() const noexcept;

};


class
token
{
  token_info  m_info;

  enum class kind{
    null,
    identifier,
    single_quoted,
    double_quoted,
    binary_number,
    octal_number,
    decimal_number,
    hexadecimal_number,
    floating_point_number,
    operator_code,
    block,

  } m_kind=kind::null;

  union data{
    uint64_t     n;
    std::string  s;
    char         c;
    double       f;

    operator_code  opco;

    token_block  blk;

    data() noexcept{}
   ~data()         {}
  } m_data;


  static void  print_string(const std::string&  s, char  punct) noexcept;

public:
  token() noexcept{}
  token(const token&   rhs) noexcept{assign(rhs);}
  token(      token&&  rhs) noexcept{assign(std::move(rhs));}
  token(token_info  info, uint64_t  n, int  sym)      noexcept{assign(info,n,sym);}
  token(token_info  info, double  f)                  noexcept{assign(info,f);}
  token(token_info  info, std::string&&  s, int  sym) noexcept{assign(info,std::move(s),sym);}
  token(token_info  info, operator_code  opco)        noexcept{assign(info,opco);}
  token(token_info  info, token_block&&  blk)         noexcept{assign(info,std::move(blk));}
 ~token(){clear();}

  token&  operator=(const token&   rhs) noexcept{return assign(rhs);}
  token&  operator=(      token&&  rhs) noexcept{return assign(std::move(rhs));}

  token&  assign(const token&   rhs) noexcept;
  token&  assign(      token&&  rhs) noexcept;
  token&  assign(token_info  info, uint64_t  n, int  sym) noexcept;
  token&  assign(token_info  info, double  f) noexcept;
  token&  assign(token_info  info, std::string&&  s, int  sym) noexcept;
  token&  assign(token_info  info, operator_code  opco) noexcept;
  token&  assign(token_info  info, token_block&&  blk) noexcept;

  void  clear() noexcept;

  const token_info&  get_info() const noexcept{return m_info;}

  bool  is_null() const noexcept{return m_kind == kind::null;}
  bool  is_binary_number() const noexcept{return m_kind == kind::binary_number;}
  bool  is_octal_number() const noexcept{return m_kind == kind::octal_number;}
  bool  is_decimal_number() const noexcept{return m_kind == kind::decimal_number;}
  bool  is_hexadecimal_number() const noexcept{return m_kind == kind::hexadecimal_number;}
  bool  is_identifier() const noexcept{return m_kind == kind::identifier;}
  bool  is_single_quoted() const noexcept{return m_kind == kind::single_quoted;}
  bool  is_double_quoted() const noexcept{return m_kind == kind::double_quoted;}
  bool  is_operator_code() const noexcept{return m_kind == kind::operator_code;}
  bool  is_floating_point_number() const noexcept{return m_kind == kind::floating_point_number;}

  uint64_t            get_number()                const noexcept{return m_data.n;}
  double              get_floating_point_number() const noexcept{return m_data.f;}
  const std::string&  get_string()                const noexcept{return m_data.s;}
  operator_code       get_operator_code()         const noexcept{return m_data.opco;}

  void  print() const noexcept;

};


class
tokenizer
{
  int  m_line_counter=0;

  const char*  m_pointer=nullptr;

  token_info  m_info;

  token       read_binary_number() noexcept;
  token        read_octal_number() noexcept;
  token      read_decimal_number() noexcept;
  token  read_hexadecimal_number() noexcept;

  token  read_number_that_begins_by_zero() noexcept;
  token  read_number() noexcept;

  token  read_operator_code() noexcept;

  token  read_quoted_string(char  close_char);

  token  read_block(operator_code  open, operator_code  close);


  void  skip_spaces();

  void  skip_linestyle_comment() noexcept;
  void  skip_blockstyle_comment();


  bool  step(std::vector<token>&  toks, operator_code  close, int close_len);

  void  update_info() noexcept{m_info = token_info(m_pointer,m_line_counter);}

public:
  std::vector<token>  operator()(const char*  p);

};




}




#endif




