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


using execution_callback = void(*)(execution&,void*);



struct
execution_frame
{
  std::string  m_name;

  execution_callback  m_callback;

  void*  m_data;

  execution_frame(const char*  name, execution_callback  cb=nullptr, void*  data=nullptr) noexcept:
  m_name(name), m_callback(cb), m_data(data){}

  template<typename  T>
  execution_frame(const char*  name, void(*cb)(execution&,T*), T*  data) noexcept:
  m_name(name), m_callback(reinterpret_cast<execution_callback>(cb)), m_data(data){}

  void  operator()(execution&  exec) const noexcept{m_callback(exec,m_data);}

};


class
execution
{
  std::vector<execution_frame>  m_stack;

public:
  operator bool() const noexcept{return m_stack.size();}

  void     push(execution_frame&&  f) noexcept{m_stack.emplace_back(std::move(f));}
  void  replace(execution_frame&&  f) noexcept{m_stack.back() = std::move(f);}

  void    pop() noexcept{m_stack.pop_back();}
  void  clear() noexcept{m_stack.clear();}

  execution_callback  get_top() const noexcept{return m_stack.size()? m_stack.back().m_callback:nullptr;}

  bool  test(execution_callback  cb) const noexcept{return m_stack.size() && (m_stack.back().m_callback == cb);}

  void  operator()(int  n=1) noexcept
  {
      while(n--)
      {
        auto&  bk = m_stack.back();

        auto  last = bk.m_callback;

        bk(*this);

          if(m_stack.empty() || (m_stack.back().m_callback == last))
          {
            break;
          }
      }
  }

};




class
process
{
  std::string  m_name;

  uint32_t  m_next_time=0;
  uint32_t  m_interval=0;

  execution  m_execution;

public:
  process() noexcept{}
  process(const char*  name, uint32_t  intval, execution_frame&&  frame) noexcept{assign(name,intval,std::move(frame));}
  process(const process&   rhs) noexcept=delete;
  process(      process&&  rhs) noexcept{assign(std::move(rhs));}
 ~process(){clear();}

  process&  operator=(const process&   rhs) noexcept=delete;
  process&  operator=(      process&&  rhs) noexcept{return assign(std::move(rhs));}

  process&  assign(process&&  rhs) noexcept;
  process&  assign(const char*  name, uint32_t  intval, execution_frame&&  frame) noexcept;


  void  clear() noexcept;

  uint32_t  get_interval() const noexcept{return m_interval;}

  const execution&  get_execution() const noexcept{return m_execution;}

  void  step() noexcept;

};




}




#endif




