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

};



struct
execution_entry
{
  execution_callback  m_callback;

  void*  m_data;

  stop_sign  m_stop_sign;

  constexpr execution_entry() noexcept:
  m_callback(nullptr), m_data(nullptr), m_stop_sign(false){}

  template<typename  T>
  constexpr execution_entry(void(*cb)(execution&,T*), T*  data, stop_sign  ss=false) noexcept:
  m_callback(reinterpret_cast<execution_callback>(cb)), m_data(data), m_stop_sign(ss){}

};


class
execution_frame
{
  static constexpr int  m_capacity = 12;

  execution_entry  m_entries[m_capacity];

  int  m_length=0;

public:
  execution_frame(std::initializer_list<execution_entry>  ls={}) noexcept{assign(ls);}

  execution_frame&  operator=(std::initializer_list<execution_entry>  ls) noexcept{return assign(ls);}

  execution_frame&  assign(std::initializer_list<execution_entry>  ls) noexcept
  {
    m_length = ls.size();

    auto  it = ls.begin();

      for(int  i = 0;  i < ls.size();  ++i)
      {
        m_entries[i] = *it++;
      }


    return *this;
  }

  int  get_length() const noexcept{return m_length;}

  const execution_entry&  operator[](int  i) const noexcept{return m_entries[i];}

};


class
execution
{
protected:
  struct frame: public execution_frame{
    uint32_t  m_pc;

    frame(std::initializer_list<execution_entry>  ls) noexcept:
    execution_frame(ls), m_pc(0){}

    frame&  assign(std::initializer_list<execution_entry>  ls) noexcept
    {
      execution_frame::assign(ls);

      m_pc = 0;

      return *this;
    }

  };

  std::vector<frame>  m_main_stack;
  std::vector<frame>  m_buffer_stack;

  bool  m_pop_flag=false;

   execution() noexcept{}
  ~execution() noexcept{}

  execution(const execution& )=default;
  execution(      execution&&)=default;

  execution&  operator=(const execution& )=default;
  execution&  operator=(      execution&&)=default;

public:
  operator bool() const noexcept{return m_main_stack.size();}

  int  get_number_of_frames() const noexcept{return m_main_stack.size();}

  void     push(std::initializer_list<execution_entry>  ls) noexcept{m_buffer_stack.emplace_back(ls);}
  void  replace(std::initializer_list<execution_entry>  ls) noexcept{m_main_stack.back().assign(ls);}

  uint32_t  get_pc(           ) const noexcept{return m_main_stack.back().m_pc     ;}
  void      set_pc(uint32_t  v)       noexcept{       m_main_stack.back().m_pc  = v;}
  void      add_pc(int       v)       noexcept{       m_main_stack.back().m_pc += v;}

  execution&  operator++() noexcept
  {
    m_main_stack.back().m_pc += 1;

    return *this;
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
  process(uint32_t  interval, std::initializer_list<execution_entry>  ls) noexcept
  {assign(interval,ls);}

  bool  is_busy() const noexcept{return m_main_stack.size() || m_buffer_stack.size();}

  process&  assign(uint32_t  interval, std::initializer_list<execution_entry>  ls) noexcept;

  void      set_interval(uint32_t  v)       noexcept{       m_interval = v;}
  uint32_t  get_interval(           ) const noexcept{return m_interval    ;}

  void  step() noexcept;

};




}




#endif




