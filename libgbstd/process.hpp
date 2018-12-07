#ifndef gbstd_job_HPP
#define gbstd_job_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include<utility>
#include<string>
#include<vector>
#include<cstdint>


namespace gbstd{


class  process;
class  execution;


using base_callback = void(*)(execution&,void*);


class
execution
{
  struct frame{
    base_callback  m_callback;

    frame(base_callback  cb=nullptr) noexcept:
    m_callback(cb){}

  };

  std::vector<frame>  m_stack;

public:
  operator bool() const noexcept{return m_stack.size();}

  template<typename  T>void  push(void(*cb)(execution&,T*)) noexcept
  {
    m_stack.emplace_back(reinterpret_cast<base_callback>(cb));
  }

  template<typename  T>void  replace(void(*cb)(execution&,T*)) noexcept
  {
    m_stack.back() = frame(reinterpret_cast<base_callback>(cb));
  }

  void    pop() noexcept{m_stack.pop_back();}
  void  clear() noexcept{m_stack.clear();}

  base_callback  get() const noexcept{return m_stack.size()? m_stack.back().m_callback:nullptr;}

};




class
process
{
  std::string  m_name;

  uint32_t  m_next_time=0;
  uint32_t  m_interval=0;

  void*  m_data=nullptr;

  base_callback  m_base_callback=nullptr;

  execution  m_execution;

public:
  process() noexcept{}
  process(const char*  name, uint32_t  intval, base_callback  cb, void*  dat=nullptr) noexcept{assign(name,intval,cb,dat);}
  process(const process&   rhs) noexcept=delete;
  process(      process&&  rhs) noexcept{assign(std::move(rhs));}
 ~process(){clear();}

  process&  operator=(const process&   rhs) noexcept=delete;
  process&  operator=(      process&&  rhs) noexcept{return assign(std::move(rhs));}

  process&  assign(process&&  rhs) noexcept;
  process&  assign(const char*  name, uint32_t  intval, base_callback  cb, void*  dat=nullptr) noexcept;

  template<typename  T>
  process(const char*  name, uint32_t  intval, void  (*cb)(execution&,T*), T*  dat) noexcept
  {
    assign(name,intval,reinterpret_cast<base_callback>(cb),dat);
  }

  template<typename  T>
  process&
  assign(const char*  name, uint32_t  intval, void  (*cb)(execution&,T*), T*  dat) noexcept
  {
    return assign(name,intval,reinterpret_cast<base_callback>(cb),dat);
  }


  void  clear() noexcept;

  uint32_t  get_interval() const noexcept{return m_interval;}

  const std::string&  get_name() const noexcept{return m_name;}

  const execution&  get_execution() const noexcept{return m_execution;}

  void  step() noexcept;

};




}




#endif




