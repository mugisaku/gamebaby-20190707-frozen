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

using execution_callback = void(*)(execution&,dummy&);


class
interruption
{
  bool  m_data;

public:
  explicit constexpr interruption(bool  v=false) noexcept: m_data(v){}

  constexpr operator bool() const noexcept{return m_data;}

};


namespace interruptions{
constexpr interruption   on( true);
constexpr interruption  off(false);
}


struct
execution_entry
{
  uintptr_t  m_nameptr;
  uintptr_t  m_callback;
  uintptr_t  m_data;
  uintptr_t  m_interruption;

  template<typename  T> constexpr uintptr_t  cast(T*  ptr) noexcept{return reinterpret_cast<uintptr_t>(ptr);}
                        constexpr uintptr_t  cast(interruption  i) noexcept{return static_cast<uintptr_t>(i);}

  constexpr execution_entry() noexcept:
  m_nameptr(0), m_callback(0), m_data(0), m_interruption(0){}

  template<typename  T>
  constexpr execution_entry(void(*cb)(execution&,T&), T&  data, interruption  i=interruptions::off) noexcept:
  m_nameptr(0), m_callback(cast(cb)), m_data(cast(&data)), m_interruption(cast(i)){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&,T&), T&  data, interruption  i=interruptions::off) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(cast(&data)), m_interruption(cast(i)){}

  template<typename  T>
  constexpr execution_entry(const char*  name, T&  data, interruption  i=interruptions::off) noexcept:
  m_nameptr(cast(name)), m_callback(cast(T::execute)), m_data(cast(&data)), m_interruption(cast(i)){}

  constexpr const char*   get_name()         const noexcept{return m_nameptr? reinterpret_cast<const char*>(m_nameptr):"";}
  constexpr interruption  get_interruption() const noexcept{return interruption(m_interruption);}

  execution_callback  get_callback()  const noexcept{return reinterpret_cast<execution_callback>(m_callback);}
  dummy*                  get_data()      const noexcept{return reinterpret_cast<dummy*>(m_data);}

};


class
execution
{
protected:
  std::string  m_name;

  uint32_t  m_pc=0;//Program Counter
  uint32_t  m_lc=0;//Level Counter
  uint32_t  m_sp=0;//Stack Pointer
  uint32_t  m_bp=0;//Base Pointer

  std::vector<uintptr_t>  m_memory;

  bool  m_verbose_flag=false;

  bool  m_pc_barrier;

   execution() noexcept: m_memory(800) {}
  ~execution() noexcept{}

  execution(const execution& )=default;
  execution(      execution&&)=default;

  execution&  operator=(const execution& )=default;
  execution&  operator=(      execution&&)=default;

public:
  operator bool() const noexcept{return m_lc;}

  const char*  get_name() const noexcept{return m_name.size()? m_name.data():"NoName";}

  bool  test_verbose_flag() const noexcept{return m_verbose_flag;}
  void    set_verbose_flag() noexcept{m_verbose_flag =  true;}
  void  unset_verbose_flag() noexcept{m_verbose_flag = false;}

  const uint32_t&  get_pc() const noexcept{return m_pc;}
  const uint32_t&  get_lc() const noexcept{return m_lc;}
  const uint32_t&  get_sp() const noexcept{return m_sp;}
  const uint32_t&  get_bp() const noexcept{return m_bp;}

  void     push(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;
  void  replace(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;

  void  pop() noexcept;

  execution&  operator++() noexcept;

  void  print() const noexcept;

};


class
process: public execution
{
public:
  process() noexcept{}
  process(std::initializer_list<execution_entry>  ls) noexcept{assign(ls);}

  bool  is_busy() const noexcept{return *this;}

  process&  assign(std::initializer_list<execution_entry>  ls) noexcept;

  void  step() noexcept;

};




}




#endif




