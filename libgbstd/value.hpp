#ifndef gbstd_value_HPP
#define gbstd_value_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include<new>
#include<utility>
#include<array>
#include<list>
#include<vector>
#include<string>
#include<cstdint>


namespace gbstd{


class variable;
class value_array;
class value_list;


class
value
{
  enum class kind{
    null,
    boolean,
    integer,
    string,
    real_number,
    user_input,
    variable,
    list,
    array,

  } m_kind=kind::null;

  union data{
    bool           b;
    int            i;
    std::string    s;
    double         r;
    user_input    ui;

    variable*        var;
    value_array*     arr;
    value_list*       ls;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(bool  b) noexcept{assign(b);}
  value(int  i) noexcept{*this = i;}
  value(const char*  s) noexcept{*this = s;}
  value(user_input  ui)      noexcept{*this = ui;}
  value(variable&&  var) noexcept{assign(std::move(var));}
  value(value_array&&  arr) noexcept{assign(std::move(arr));}
  value(value_list&&  ls) noexcept{assign(std::move(ls));}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(bool  b) noexcept{return assign(b);}
  value&  operator=(int  i) noexcept{return assign(i);}
  value&  operator=(const char*  s) noexcept{return assign(s);}
  value&  operator=(user_input  ui) noexcept{return assign(ui);}
  value&  operator=(variable&&  var) noexcept{return assign(std::move(var));}
  value&  operator=(value_array&&  arr) noexcept{return assign(std::move(arr));}
  value&  operator=(value_list&&  ls) noexcept{return assign(std::move(ls));}
  value&  operator=(const value&   rhs) noexcept{return assign(rhs);}
  value&  operator=(      value&&  rhs) noexcept{return assign(std::move(rhs));}

  value&  assign(bool  b) noexcept;
  value&  assign(int  i) noexcept;
  value&  assign(const char*  s) noexcept;
  value&  assign(user_input  ui) noexcept;
  value&  assign(variable&&  var) noexcept;
  value&  assign(value_array&&  arr) noexcept;
  value&  assign(value_list&&  ls) noexcept;
  value&  assign(const value&   rhs) noexcept;
  value&  assign(      value&&  rhs) noexcept;

  void  clear() noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_boolean()    const noexcept{return m_kind == kind::boolean;}
  bool  is_integer()    const noexcept{return m_kind == kind::integer;}
  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_user_input() const noexcept{return m_kind == kind::user_input;}
  bool  is_variable()   const noexcept{return m_kind == kind::variable;}
  bool  is_array()      const noexcept{return m_kind == kind::array;}
  bool  is_list()       const noexcept{return m_kind == kind::list;}

  bool                get_boolean()    const noexcept{return m_data.b;}
  int                 get_integer()    const noexcept{return m_data.i;}
  const std::string&  get_string()     const noexcept{return m_data.s;}
  user_input          get_user_input() const noexcept{return m_data.ui;}
  variable&           get_variable()   const noexcept{return *m_data.var;}
  value_array&        get_array()      const noexcept{return *m_data.arr;}
  value_list&         get_list()       const noexcept{return *m_data.ls;}

  void  print() const noexcept;

};


class
variable
{
  std::string  m_name;

  value  m_value;

public:
  variable() noexcept{}
  variable(const char*  name, const value&   v) noexcept{assign(name,          v );}
  variable(const char*  name,       value&&  v) noexcept{assign(name,std::move(v));}

  variable&  assign(const char*  name, const value&   v) noexcept;
  variable&  assign(const char*  name,       value&&  v) noexcept;

  const std::string&  get_name(                 ) const noexcept{return m_name       ;}
  void                set_name(const char*  name)       noexcept{       m_name = name;}

        value&  get_value()       noexcept{return m_value;}
  const value&  get_value() const noexcept{return m_value;}

  void  set_value(const value&   v) noexcept{m_value =           v ;}
  void  set_value(      value&&  v) noexcept{m_value = std::move(v);}

  void  print() const noexcept;

};


class
value_array
{
  value*  m_data=nullptr;

  size_t  m_number_of_elements=0;

  size_t  m_length=0;

  void  reallocate(size_t  n) noexcept;

public:
  value_array() noexcept{}
  value_array(const value_array&   rhs) noexcept{assign(rhs);}
  value_array(      value_array&&  rhs) noexcept{assign(std::move(rhs));}
 ~value_array(){clear();}

  value_array&  operator=(const value_array&   rhs) noexcept{return assign(rhs);}
  value_array&  operator=(      value_array&&  rhs) noexcept{return assign(std::move(rhs));}

  value&  operator[](int  i) const noexcept{return m_data[i];}

  value_array&  assign(const value_array&   rhs) noexcept;
  value_array&  assign(      value_array&&  rhs) noexcept;

  void  clear() noexcept;

  void  resize(size_t  n) noexcept;

  value&  push(value  v) noexcept;

  value&   pop()       noexcept;
  value&  back() const noexcept{return m_data[m_length-1];}

  variable*  find_variable(const char*  name) const noexcept;

  size_t  size() const noexcept{return m_length;}

  value*  begin() const noexcept{return m_data;}
  value*    end() const noexcept{return m_data+m_length;}

  void  print() const noexcept;

};


class
value_list
{
  struct node{
    value  m_value;

    node*  m_prev;
    node*  m_next;

    node(value&&  v) noexcept: m_value(std::move(v)){}
  };

  node*  m_first=nullptr;
  node*  m_last =nullptr;

  size_t  m_length=0;

public:
  value_list() noexcept{}
  value_list(const value_list&   rhs) noexcept{assign(rhs);}
  value_list(      value_list&&  rhs) noexcept{assign(std::move(rhs));}
 ~value_list(){clear();}

  value_list&  operator=(const value_list&   rhs) noexcept{return assign(rhs);}
  value_list&  operator=(      value_list&&  rhs) noexcept{return assign(std::move(rhs));}

  value_list&  assign(const value_list&   rhs) noexcept;
  value_list&  assign(      value_list&&  rhs) noexcept;

  void  clear() noexcept;

  void  resize(size_t  n) noexcept;

  value&  push_front(value  v) noexcept;
  value&  push_back(value  v)  noexcept;

  value   pop_back()  noexcept;
  value   pop_front() noexcept;

  value&  front() const noexcept{return m_first->m_value;}
  value&   back() const noexcept{return m_last->m_value;}

  variable*  find_variable(const char*  name) const noexcept;

  size_t  size() const noexcept{return m_length;}

  void  print() const noexcept;


  class iterator{
    node*  m_node=nullptr;

    friend class value_list;

  public:
    constexpr iterator() noexcept{}
    constexpr iterator(node*  nd) noexcept: m_node(nd){}

    constexpr operator bool() const noexcept{return m_node;}

    constexpr bool  operator==(node*  nd) const noexcept{return m_node == nd;}
    constexpr bool  operator!=(node*  nd) const noexcept{return m_node != nd;}

    value&  operator*() const noexcept{return m_node->m_value;}

    iterator&  operator++() noexcept
    {
      m_node = m_node->m_next;

      return *this;
    }

    iterator&  operator--() noexcept
    {
      m_node = m_node->m_prev;

      return *this;
    }

    iterator  operator++(int) noexcept
    {
      auto  tmp = *this;

      m_node = m_node->m_next;

      return tmp;
    }

    iterator  operator--(int) noexcept
    {
      auto  tmp = *this;

      m_node = m_node->m_prev;

      return tmp;
    }

  };


  iterator  begin() const noexcept{return iterator(m_first);}
  iterator    end() const noexcept{return iterator(       );}


  value&  insert_before(iterator  it, value  v) noexcept;
  value&  insert_after( iterator  it, value  v) noexcept;

  iterator  erase(iterator  it) noexcept;

};




}




#endif




