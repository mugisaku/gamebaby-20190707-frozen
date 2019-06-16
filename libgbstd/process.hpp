#ifndef gbstd_process_HPP
#define gbstd_process_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/clock.hpp"
#include"libgbstd/task.hpp"
#include<utility>
#include<string>
#include<vector>
#include<cstdint>


namespace gbstd{


class execution;
class process;

using execution_callback = void(*)(execution&,dummy&);


struct
execution_entry
{
  uintptr_t  m_nameptr;
  uintptr_t  m_callback;
  uintptr_t  m_data;

  template<typename  T> constexpr uintptr_t  cast(T*  ptr) noexcept{return reinterpret_cast<uintptr_t>(ptr);}

  constexpr execution_entry() noexcept:
  m_nameptr(0), m_callback(0), m_data(0){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&)) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(0){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&,T&), T&  data) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(cast(&data)){}

  template<typename  T>
  constexpr execution_entry(const char*  name, T&  data) noexcept:
  m_nameptr(cast(name)), m_callback(cast(T::execute)), m_data(cast(&data)){}

  constexpr const char*  get_name() const noexcept{return m_nameptr? reinterpret_cast<const char*>(m_nameptr):"";}

  execution_callback  get_callback()  const noexcept{return reinterpret_cast<execution_callback>(m_callback);}
  dummy*                  get_data()      const noexcept{return reinterpret_cast<dummy*>(m_data);}

  constexpr bool  test_interruption() const noexcept{return get_name()[0] == '*';}

};


class
execution
{
protected:
  clock_master  m_clock_master;

  task_list  m_task_list;

  color  m_background_color;

  uint32_t  m_pc=0;//Program Counter
  uint32_t  m_lc=0;//Level Counter
  uint32_t  m_sp=0;//Stack Pointer
  uint32_t  m_bp=0;//Base Pointer

  std::vector<uintptr_t>  m_memory;

  bool  m_verbose_flag=false;
  bool  m_pc_barrier=false;

   execution() noexcept: m_memory(800){}
  ~execution(){}

  execution(const execution& )=default;
  execution(      execution&&)=default;

  execution&  operator=(const execution& )=default;
  execution&  operator=(      execution&&)=default;

  execution&  reset() noexcept;

public:
  operator bool() const noexcept{return m_lc;}

  bool  test_verbose_flag() const noexcept{return m_verbose_flag;}
  execution&    set_verbose_flag() noexcept{  m_verbose_flag =  true;  return *this;}
  execution&  unset_verbose_flag() noexcept{  m_verbose_flag = false;  return *this;}

  execution&  add_clock(clock&  cl) noexcept{  m_clock_master.add(cl);  return *this;}
  execution&   add_task(task&  tsk) noexcept{  m_task_list.add(tsk);    return *this;}

  execution&  remove_clock(std::string_view  name) noexcept{  m_clock_master.remove(name);  return *this;}
  execution&   remove_task(std::string_view  name) noexcept{  m_task_list.remove_by_name(name);  return *this;}

  color       get_background_color(        ) const noexcept{return m_background_color                   ;}
  execution&  set_background_color(color  c)       noexcept{       m_background_color = c;  return *this;}

  const uint32_t&  get_pc() const noexcept{return m_pc;}
  const uint32_t&  get_lc() const noexcept{return m_lc;}
  const uint32_t&  get_sp() const noexcept{return m_sp;}
  const uint32_t&  get_bp() const noexcept{return m_bp;}

  execution&     push(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;
  execution&  replace(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;

  execution&  pop() noexcept;

  execution&  operator++() noexcept;

  void  print() const noexcept;

};


class
process: public execution
{
  std::string  m_name;

  uint32_t  m_interval=0;
  uint32_t  m_next_time=0;

  canvas  m_canvas;

public:
  process() noexcept{}

  bool  is_busy() const noexcept{return *this;}

  std::string_view  get_name() const noexcept{return m_name.size()? m_name:"NoName";}

  process&  assign(std::string_view  name, execution_entry  start) noexcept;

  process&  set_canvas(const canvas&  cv) noexcept{  m_canvas = cv;  return *this;}
  process&  set_interval(uint32_t  t) noexcept{  m_interval = t;  return *this;}

  bool  step() noexcept;

};




}




#endif




