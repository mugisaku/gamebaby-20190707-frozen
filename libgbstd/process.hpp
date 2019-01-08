#ifndef gbstd_process_HPP
#define gbstd_process_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include<utility>
#include<string>
#include<vector>
#include<cstdint>


namespace gbstd{


class execution;
class process;

using execution_callback = void(*)(execution&,void*);




class
stop_sign
{
  bool  m_value;

public:
  constexpr stop_sign(bool  v) noexcept: m_value(v){}

  constexpr operator bool() const noexcept{return m_value;}
  constexpr operator uintptr_t() const noexcept{return m_value;}

};



struct
execution_entry
{
  uintptr_t  m_nameptr;

  uintptr_t  m_callback;

  uintptr_t  m_data;

  uintptr_t  m_stop_sign;

  template<typename  T> constexpr uintptr_t  cast(T  v) noexcept{return reinterpret_cast<uintptr_t>(v);}

  constexpr execution_entry() noexcept:
  m_nameptr(0), m_callback(0), m_data(0), m_stop_sign(0){}

  template<typename  T>
  constexpr execution_entry(void(*cb)(execution&,T*), T*  data, stop_sign  ss=false) noexcept:
  m_nameptr(0), m_callback(cast(cb)), m_data(cast(data)), m_stop_sign(ss? 1:0){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&,T*), T*  data, stop_sign  ss=false) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(cast(data)), m_stop_sign(ss? 1:0){}

  constexpr const char*         get_name()      const noexcept{return m_nameptr? reinterpret_cast<const char*>(m_nameptr):"";}
  constexpr execution_callback  get_callback()  const noexcept{return reinterpret_cast<execution_callback>(m_callback);}
  constexpr void*               get_data()      const noexcept{return reinterpret_cast<void*>(m_data);}
  constexpr stop_sign           get_stop_sign() const noexcept{return stop_sign(m_stop_sign);}

};


class
execution
{
protected:
  std::string  m_name;

  uint32_t  m_pc=0;
  uint32_t  m_sp=0;
  uint32_t  m_bp=0;

  std::vector<uintptr_t>  m_memory;

  bool  m_verbose_flag=false;

   execution() noexcept: m_memory(800) {}
  ~execution() noexcept{}

  execution(const execution& )=default;
  execution(      execution&&)=default;

  execution&  operator=(const execution& )=default;
  execution&  operator=(      execution&&)=default;

public:
  operator bool() const noexcept{return m_pc;}

  const char*  get_name() const noexcept{return m_name.size()? m_name.data():"NoName";}

  bool  test_verbose_flag() const noexcept{return m_verbose_flag;}
  void    set_verbose_flag() noexcept{m_verbose_flag =  true;}
  void  unset_verbose_flag() noexcept{m_verbose_flag = false;}

  void     push(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;
  void  replace(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;

  void  pop() noexcept;

  execution&  operator++() noexcept;

  void  print() const noexcept;

};


class
process: public execution
{
  uint32_t  m_interval=0;

  uint32_t  m_next_time=0;

  uint32_t  m_call_counter=0;

public:
  process(uint32_t  interval=0) noexcept: m_interval(interval){}
  process(uint32_t  interval, std::initializer_list<execution_entry>  ls) noexcept
  {assign(interval,ls);}

  bool  is_busy() const noexcept{return *this;}

  process&  assign(uint32_t  interval, std::initializer_list<execution_entry>  ls) noexcept;

  void      set_interval(uint32_t  v)       noexcept{       m_interval = v;}
  uint32_t  get_interval(           ) const noexcept{return m_interval    ;}

  void  step() noexcept;

};




}




#endif




