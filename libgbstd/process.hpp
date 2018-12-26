#ifndef gbstd_process_HPP
#define gbstd_process_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include<utility>
#include<string>
#include<vector>
#include<cstdint>


namespace gbstd{


class  execution;

using execution_callback = void(*)(execution&,void*);



class
execution_frame
{
  execution_callback  m_callback;

  void*  m_data;

public:
  execution_frame(execution_callback  cb=nullptr, void*  data=nullptr) noexcept:
  m_callback(cb), m_data(data){}

  template<typename  T>
  execution_frame(void(*cb)(execution&,T*), T*  data) noexcept:
  m_callback(reinterpret_cast<execution_callback>(cb)), m_data(data){}

  execution_callback  get_callback() const noexcept{return m_callback;}

  void*  get_data() const noexcept{return m_data;}

};


class
stop_sign
{
  bool  m_value;

public:
  constexpr stop_sign(bool  v) noexcept: m_value(v){}

  constexpr operator bool() const noexcept{return m_value;}

};


class
execution
{
protected:
  using base = execution_frame;

  struct frame: public base{
    stop_sign  m_stop_sign;

    frame(base&&  bas, stop_sign  stp) noexcept:
    base(std::move(bas)), m_stop_sign(stp){}

    frame&  assign(base&&  bas, stop_sign  stp) noexcept
    {
      static_cast<base&>(*this) = (std::move(bas));
      m_stop_sign = stp;

      return *this;
    }

  };

  std::vector<frame>  m_main_stack;
  std::vector<frame>  m_buffer_stack;

  bool  m_pop_flag=false;

   execution() noexcept=default;
  ~execution() noexcept=default;

  execution(const execution& ) noexcept=default;
  execution(      execution&&) noexcept=default;

  execution&  operator=(const execution& ) noexcept=default;
  execution&  operator=(      execution&&) noexcept=default;

public:
  operator bool() const noexcept{return m_main_stack.size();}

  int  get_number_of_frames() const noexcept{return m_main_stack.size();}

  void     push(execution_frame&&  f, stop_sign  stp=stop_sign(false)) noexcept{m_buffer_stack.emplace_back(std::move(f),stp);}
  void  replace(execution_frame&&  f, stop_sign  stp=stop_sign(false)) noexcept{m_main_stack.back().assign(std::move(f),stp);}


  void  push(std::initializer_list<execution_callback>  ls, void*  data, stop_sign  stp=stop_sign(false)) noexcept
  {
      for(auto  cb: ls)
      {
        push({cb,data},stp);
      }
  }


  template<typename  T>
  void  push(std::initializer_list<void(*)(execution&,T*)>  ls, T*  data, stop_sign  stp=stop_sign(false)) noexcept
  {
      for(auto  cb: ls)
      {
        push({cb,data},stp);
      }
  }


  void  push_reverse(std::initializer_list<execution_callback>  ls, void*  data, stop_sign  stp=stop_sign(false)) noexcept
  {
    auto  end = ls.begin();
    auto   it = ls.end();

      if(it != end)
      {
          for(;;)
          {
            push({*--it,data},stp);

              if(it == end)
              {
                break;
              }
          }
      }
  }


  template<typename  T>
  void  push_reverse(std::initializer_list<void(*)(execution&,T*)>  ls, T*  data, stop_sign  stp=stop_sign(false)) noexcept
  {
    auto  end = ls.begin();
    auto   it = ls.end();

      if(it != end)
      {
          for(;;)
          {
            push({*--it,data},stp);

              if(it == end)
              {
                break;
              }
          }
      }
  }


  void  pop() noexcept{m_pop_flag = true;}

};


class
process: public execution
{
  uint32_t  m_interval=0;

  uint32_t  m_next_time=0;

  void  merge() noexcept;

public:
  process(uint32_t  interval=0) noexcept: m_interval(interval){}
  process(uint32_t  interval, execution_frame&&  frame, stop_sign  stp=stop_sign(false)) noexcept
  {assign(interval,std::move(frame),stp);}

  bool  is_busy() const noexcept{return m_main_stack.size() || m_buffer_stack.size();}

  process&  assign(uint32_t  interval, execution_frame&&  frame, stop_sign  stp=stop_sign(false)) noexcept;

  void      set_interval(uint32_t  v)       noexcept{       m_interval = v;}
  uint32_t  get_interval(           ) const noexcept{return m_interval    ;}

  void  step() noexcept;

};


inline
int
wait(std::initializer_list<process*>  ls) noexcept
{
  int  n = 0;

    for(auto  proc: ls)
    {
        if(proc)
        {
            if(proc->is_busy())
            {
              proc->step();
            }


            if(proc->is_busy())
            {
              ++n;
            }
        }
    }


  return n;
}




}




#endif




